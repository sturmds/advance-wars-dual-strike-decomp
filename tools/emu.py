#!/usr/bin/env python3
"""Drive the game in melonDS: replay a scenario script, record one, or play.

    emu.py run [--frames 30000] [--save FILE] [--tag TAG] SCRIPT [ROM[=ELF]]
    emu.py record [--save FILE] OUT [ROM[=ELF]]
    emu.py play [--save FILE] [ROM[=ELF]]

ROM defaults to build/AWRE.nds with build/arm9.elf. Symbols come from ELF,
from config/AWRE/arm9/ when the ROM is retail, or from the arm9.elf beside
the ROM. Each mode starts from an empty save unless --save gives one to copy.
melonDS runs with a config directory, home and save directory of its own
under build/emu/ (tools/melonds.py), so firmware settings, RTC and the rest
stay fixed and a run is repeatable.

run: plays SCRIPT with no window, as fast as melonDS goes (about four
times a DS), and checks its expect lines. Keys are given at the tail call of
Keypad_Poll (Keypad_Poll+0x30, `bx ip`), where r1 holds the keys of this DS
only: the tool stops there every polled frame and sets r1 to the keys the
script holds. Output goes to build/emu/<script stem>/: <tag>.log (the trace
printed), <tag>-NAME.png for each shot, <tag>-fail.png when a step fails.
TAG is the ROM's path in the checkout (orig-AWRE, build-AWRE), or for a ROM
elsewhere its stem and a hash of its path, so runs of different ROMs never
overwrite each other. melonDS runs on Qt's VNC platform (a 640x640 virtual
screen on a free port, listening on all interfaces like the GDB stub) and
the screens are grabbed over RFB; that needs PIL. Without PIL a script with
no shot lines runs offscreen, with no screenshot on failure. Exit 0 when
every step passes, 1 when one fails, 2 when the script or the names in it
are wrong or melonDS could not be run.

record: opens a melonDS window with the frame limiter on, your key and
joystick bindings (only the [Instance0.Keyboard] and [Instance0.Joystick]
tables and Instance0.JoystickID of $XDG_CONFIG_HOME/melonDS/melonDS.toml,
read-only) and the same fixed settings as run, and writes OUT while you
play: a `VBLANK KEYS` line each time the keys held change, and a
`# @ FUNCTION (frame N)` comment the first time one of RECORD_MARKERS runs
after a change, to help turn the recording into anchored steps. It stops
when the window closes and ends the file with `VBLANK end`. Touch input is
not recorded. A recording replays exactly only on the ROM it was made on,
from the same save. Exit 2 when melonDS could not be run.

play: opens a melonDS window as record does, with nothing scripted. The
save is left in build/emu/play/save/ for --save.

Scripts: one step per line; `#` starts a comment.

    [TIME] ACTION

TIME is a VBlank count (absolute, as recordings write), `+N` (N frames after
the previous step ended), or nothing (+0). VBlank counts come from the
counter the SDK keeps at 0x027ffc3c. The game polls no keys in some VBlanks
while it loads, so a key change applies from the first polled frame at or after
its time. ACTION is one of:

    KEYS                     hold KEYS from now on (- for none); the recording form
    hold KEYS                the same
    press KEYS [for N] [until WHAT [tries N] [every N]]
                             hold KEYS for N polled frames (default 4) on top of the
                             held keys. With until, press again N frames (every,
                             default 30) after each release until WHAT happens
                             (tries, default 10). WHAT may happen before the first
                             press; then there is none. Ends at the release or when
                             WHAT happens, whichever is later. A press waits for a
                             polled frame with its keys up, so the game sees each
                             press as a new one.
    @ WHAT [within N]        wait until WHAT happens after this step started. Ends then.
    expect reach FUNCTION within N
                             the same for a function, failing after N frames
    expect COND              check COND now
    shot NAME                save both screens as <tag>-NAME.png, a few frames on
    end                      stop here; the run passes

WHAT is a FUNCTION (the next fresh hit of its breakpoint, one after the step
started) or a COND, checked once per polled frame at the key hook. COND is

    mem SYMBOL[+OFF] TYPE[&MASK] OP VALUE

which reads TYPE (u8 u16 u32 s8 s16 s32) at a data symbol plus OFF, ands it
with MASK, and compares it (== != < <= > >=) with VALUE. Numbers may be
hex. KEYS are a, b, select, start, right, left, up, down, r, l, x, y,
joined with + (a+b).

Every FUNCTION and SYMBOL is looked up before melonDS starts; an unknown
one stops the run. A failed step prints where the ARM9 is (pc and lr,
sampled once the game runs on) and writes <tag>-fail.png. ov000 and ov004
load at the same address, and so do ov001, ov002, ov003, ov005 and ov006:
a breakpoint on an overlay function also stops in another overlay's code
when that one is loaded. The run prints a note for each such FUNCTION;
anchor on main or ITCM functions where you can.
"""

import argparse
import hashlib
import operator
import os
import re
import signal
import socket
import struct
import sys
import time
from dataclasses import dataclass, field
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from gdb_rsp import GdbClient, ProtocolError  # noqa: E402
from melonds import (MELONDS, ROOT, SIGILL, VBLANK_COUNT, VECTORS, Symbols, config,  # noqa: E402
                     describe_exception, format_keys, free_ports, launch, overlay_ranges, parse_keys,
                     parse_rom_arg, prepare, stop_melonds, user_bindings)

OUT = ROOT / "build/emu"
DEFAULT_ROM = "build/AWRE.nds=build/arm9.elf"

HOOK = ("Keypad_Poll", 0x30)  # `bx ip` to Keypad_UpdateState, with the local keys in r1
HOOK_INSN = 0xE12FFF1C  # bx ip
KEY_MASK = 0xFFF  # r1 also holds the debug key, bit 13
HOLD = 4  # polled frames a press lasts
RETRY = 30  # frames after a release before an `until` press is tried again
TRIES = 10
SHOT_DELAY = 6  # frames run after a shot step before the grab; melonDS repaints every few frames
HANG_FRAMES = 600  # VBlanks without a key poll that count as a hang
POLL = 0.25  # seconds to wait for a stop before interrupting
VNC_SIZE = 640
RECORD_MARKERS = (
    "Ov004Task_RunPressStartScreen", "MainMenu_Setup", "MainMenu_BuildPage2", "MapSelect_Init",
    "MapSelect_ConfirmMap", "CoSelectScreen_Create", "CoSelectScreen_BeginExit", "ChoicePrompt_Update",
    "func_020e88ac", "Game_State15_RunStep")

TYPES = {"u8": "<B", "u16": "<H", "u32": "<I", "s8": "<b", "s16": "<h", "s32": "<i"}
OPS = {"==": operator.eq, "!=": operator.ne, "<": operator.lt, "<=": operator.le, ">": operator.gt,
       ">=": operator.ge}
NAME = re.compile(r"^[A-Za-z0-9_.-]+$")


# Scripts

class ScriptError(Exception):
    pass


@dataclass
class Cond:
    """`mem SYMBOL[+OFF] TYPE[&MASK] OP VALUE`: a value in memory compared with a number."""
    symbol: str
    offset: int
    type: str
    mask: int | None
    op: str
    value: int

    def test(self, data: dict[str, int], target: "Target") -> tuple[bool, int]:
        """(holds, the value read)."""
        fmt = TYPES[self.type]
        value, = struct.unpack(fmt, target.read(data[self.symbol] + self.offset, struct.calcsize(fmt)))
        if self.mask is not None:
            value &= self.mask
        return OPS[self.op](value, self.value), value


@dataclass
class Step:
    line: int
    text: str
    kind: str  # hold, press, wait, reach, expect, shot, end
    at: int | None = None  # absolute VBlank
    delay: int = 0  # +N
    keys: int = 0
    hold: int = HOLD
    every: int = RETRY
    tries: int = TRIES
    function: str | None = None  # wait, reach, press until
    cond: Cond | None = None  # wait, expect, press until
    within: int | None = None
    name: str = ""  # shot


def number(text: str, what: str) -> int:
    try:
        return int(text, 0)
    except ValueError:
        raise ScriptError(f"{what} {text!r} is not a number") from None


def keys_arg(text: str) -> int:
    try:
        return parse_keys(text)
    except ValueError as e:
        raise ScriptError(str(e)) from None


def parse_cond(words: list[str]) -> Cond:
    """Takes `mem SYMBOL[+OFF] TYPE[&MASK] OP VALUE` off the front of words."""
    if len(words) < 5 or words[0] != "mem":
        raise ScriptError("expected: mem SYMBOL[+OFFSET] TYPE[&MASK] OP VALUE")
    _, where, kind, op, value = (words.pop(0) for _ in range(5))
    symbol, _, offset = where.partition("+")
    kind, _, mask = kind.partition("&")
    if kind not in TYPES:
        raise ScriptError(f"unknown type {kind!r} (types: {', '.join(TYPES)})")
    if op not in OPS:
        raise ScriptError(f"unknown comparison {op!r} (use {' '.join(OPS)})")
    return Cond(symbol, number(offset, "offset") if offset else 0, kind,
                number(mask, "mask") if mask else None, op, number(value, "value"))


def parse_target(words: list[str], step: Step) -> None:
    """FUNCTION or a mem condition, off the front of words, into step."""
    if not words:
        raise ScriptError("expected a FUNCTION or mem ...")
    if words[0] == "mem":
        step.cond = parse_cond(words)
    else:
        step.function = words.pop(0)


def parse_options(words: list[str], step: Step, allowed: tuple[str, ...]) -> None:
    seen = set()
    while words:
        word = words.pop(0)
        if word not in allowed or word in seen:
            raise ScriptError(f"unexpected {word!r}" + (" (given twice)" if word in seen else
                                                         f" (expected {', '.join(allowed)})"))
        seen.add(word)
        if word == "until":
            parse_target(words, step)
            continue
        if not words:
            raise ScriptError(f"{word} needs a number")
        setattr(step, {"for": "hold"}.get(word, word), number(words.pop(0), word))
    if ("tries" in seen or "every" in seen) and "until" not in seen:
        raise ScriptError("tries and every need until")
    if step.hold < 1 or step.tries < 1 or step.every < 0:
        raise ScriptError("for and tries must be at least 1, every at least 0")


def parse_step(words: list[str], line: int, text: str) -> Step:
    at, delay = None, 0
    if words[0].startswith("+"):
        delay = number(words.pop(0)[1:], "delay")
        if delay < 0:
            raise ScriptError("a delay must not be negative")
    elif words[0][0].isdigit():
        at = number(words.pop(0), "frame")
    if not words:
        raise ScriptError("a time with no action")
    step = Step(line, text, "", at=at, delay=delay)
    action, args = words[0], words[1:]
    if action not in ("hold", "press", "@", "expect", "shot", "end"):
        action, args = "hold", words  # the recording form: KEYS alone
    if action == "hold":
        if len(args) != 1:
            raise ScriptError(f"expected one KEYS argument, got {' '.join(args)!r}")
        step.kind, step.keys = "hold", keys_arg(args[0])
    elif action == "press":
        if not args:
            raise ScriptError("press needs KEYS")
        step.kind, step.keys = "press", keys_arg(args.pop(0))
        if not step.keys:
            raise ScriptError("press needs at least one key")
        parse_options(args, step, ("for", "until", "tries", "every"))
    elif action == "@":
        step.kind = "wait"
        parse_target(args, step)
        parse_options(args, step, ("within",))
    elif action == "expect":
        if args[:1] == ["reach"]:
            args.pop(0)
            step.kind = "reach"
            if not args or args[0] == "mem":
                raise ScriptError("expected: expect reach FUNCTION within N")
            step.function = args.pop(0)
            if args[:1] != ["within"] or len(args) != 2:
                raise ScriptError("expected: expect reach FUNCTION within N")
            step.within = number(args[1], "within")
        else:
            step.kind, step.cond = "expect", parse_cond(args)
            if args:
                raise ScriptError(f"unexpected {' '.join(args)!r}")
    elif action == "shot":
        if len(args) != 1 or not NAME.match(args[0]):
            raise ScriptError("expected: shot NAME (letters, digits, _ . -)")
        step.kind, step.name = "shot", args[0]
    else:
        if args:
            raise ScriptError("end takes no arguments")
        step.kind = "end"
    return step


def parse_script(text: str) -> list[Step]:
    steps, errors = [], []
    for i, raw in enumerate(text.splitlines(), 1):
        body = raw.split("#", 1)[0].strip()
        if not body:
            continue
        try:
            steps.append(parse_step(body.split(), i, body))
        except ScriptError as e:
            errors.append(f"line {i}: {e}: {body}")
    if errors:
        raise ScriptError("\n".join(errors))
    return steps


def check_names(steps: list[Step], symbols: Symbols) -> list[str]:
    """One message per name in the script that the ROM's symbols lack."""
    errors = []
    for step in steps:
        if step.function is not None and step.function not in symbols.functions:
            errors.append(f"line {step.line}: no function {step.function} in {symbols.source}")
        if step.cond is not None and step.cond.symbol not in symbols.data:
            where = ("a function, not data" if step.cond.symbol in symbols.functions
                     else f"not in {symbols.source}")
            errors.append(f"line {step.line}: data symbol {step.cond.symbol}: {where}")
    return errors


def overlay_notes(steps: list[Step], symbols: Symbols, overlays: dict[str, tuple[int, int]]) -> list[str]:
    """A note for each FUNCTION in an overlay whose address other overlays also cover: its
    breakpoint also stops in their code, when one of them is loaded."""
    notes = []
    for step in steps:
        module = symbols.modules.get(step.function or "")
        if module not in overlays:
            continue
        addr = symbols.functions[step.function]
        others = [o for o, (start, end) in overlays.items() if o != module and start <= addr < end]
        if others:
            notes.append(f"note: line {step.line}: {step.function} is in {module}; {', '.join(others)} "
                         f"also load at {addr:08x}, so a hit may be their code")
    return notes


# Running a script

class Target:
    """Memory and screenshots for the steps: screenshots over RFB from melonDS on Qt's VNC
    platform at `port` (None: no screen), saved as PREFIX-NAME.png."""

    def __init__(self, gdb: GdbClient, port: int | None, prefix: Path):
        self.gdb = gdb
        self.port = port
        self.prefix = prefix

    def read(self, addr: int, length: int) -> bytes:
        return self.gdb.read(addr, length)

    def shot(self, name: str) -> str:
        """Saves both screens; returns where, or why there is no screenshot."""
        if self.port is None:
            return "no screenshot: run without a screen"
        path = Path(f"{self.prefix}-{name}.png")
        time.sleep(0.2)  # the GUI thread repaints after the emulator stops
        try:
            crop_screens(vnc_frame(self.port)).save(path)
        except (OSError, EOFError, ValueError) as e:
            return f"no screenshot: {e}"
        return str(path.relative_to(ROOT) if path.is_relative_to(ROOT) else path)


@dataclass
class Active:
    """The state of the step being run."""
    start: int  # frame at which it started
    tries: int = 0
    applied: int = 0  # polled frames the current press has been held
    released: int | None = None  # frame after the press's last frame
    hit: int | None = None  # frame at which its function ran or its condition held
    due: int | None = None  # shot: frame of the grab


@dataclass
class Engine:
    """Runs the steps of a script from the stops of the emulator. It knows nothing of GDB:
    the caller arms the breakpoints in watched(), reports function hits with hit(), calls
    advance() at every stop, and at each key poll sets the keys that keys() returns."""
    steps: list[Step]
    functions: dict[str, int]
    data: dict[str, int]
    log: list[str] = field(default_factory=list)
    index: int = 0
    held: int = 0
    last: int = 0  # the keys of the last polled frame
    end: int = 0  # frame at which the previous step ended
    active: Active | None = None
    finished: bool = False
    failure: str | None = None

    def step(self) -> Step | None:
        return self.steps[self.index] if self.index < len(self.steps) else None

    def watched(self) -> set[int]:
        step = self.step()
        if self.active is None or step is None or step.function is None:
            return set()
        return {self.functions[step.function]}

    def hit(self, addr: int, frame: int) -> None:
        step = self.step()
        if (self.active is not None and step is not None and step.function is not None
                and self.functions[step.function] == addr and self.active.hit is None):
            self.active.hit = frame

    def keys(self, frame: int) -> int:
        """The keys for this polled frame; counts the frames of a press. A press starts only
        after a polled frame with its keys up, or the game would see one long press: after a
        press step that ended this frame, or a retry when the frames since the release had
        no key poll."""
        mask = self.held
        step, active = self.step(), self.active
        if (step is not None and step.kind == "press" and active is not None and active.released is None
                and not (active.applied == 0 and self.last & step.keys & ~self.held)):
            if active.applied == 0:
                active.tries += 1
            mask |= step.keys
            active.applied += 1
            if active.applied >= step.hold:
                active.released = frame + 1
        self.last = mask
        return mask

    def fail(self, message: str) -> None:
        self.failure = message
        self.finished = True
        self.log.append(f"FAIL {message}")

    def done(self, frame: int, note: str = "") -> None:
        step = self.steps[self.index]
        self.log.append(f"{frame:6d}  {step.text}" + (f"  ({note})" if note else ""))
        self.end = frame
        self.index += 1
        self.active = None

    def advance(self, frame: int, target: Target, polled: bool) -> None:
        """Starts the steps that are due and ends those whose condition is met at `frame`.
        polled: True when stopped at the key poll, where mem conditions of waits are checked."""
        while not self.finished:
            step = self.step()
            if step is None:
                self.finished = True
                return
            if self.active is None:
                start = step.at if step.at is not None else self.end + step.delay
                if frame < start:
                    return
                self.active = Active(start=frame)
                if not self.start_step(step, frame, target):
                    return
                continue
            if step.kind in ("wait", "press") and step.cond is not None and polled and self.active.hit is None:
                holds, _ = step.cond.test(self.data, target)
                if holds:
                    self.active.hit = frame
            if not self.continue_step(step, self.active, frame, target):
                return

    def start_step(self, step: Step, frame: int, target: Target) -> bool:
        """Starts `step`; True if it ended at once."""
        if step.kind == "hold":
            self.held = step.keys
            self.done(frame)
        elif step.kind == "expect":
            holds, value = step.cond.test(self.data, target)
            if holds:
                self.done(frame, f"{value:#x}")
            else:
                self.fail(f"line {step.line}: {step.text}: read {value:#x} ({value})")
        elif step.kind == "shot":
            self.active.due = frame + SHOT_DELAY
            return False
        elif step.kind == "end":
            self.done(frame)
            self.finished = True
        else:
            return False
        return True

    def continue_step(self, step: Step, active: Active, frame: int, target: Target) -> bool:
        """Checks the running step at `frame`; True if it ended."""
        what = step.function or "the condition"
        if step.kind in ("wait", "reach"):
            if active.hit is not None:
                self.done(active.hit, f"after {active.hit - active.start} frames")
                return True
            if step.within is not None and frame - active.start > step.within:
                self.fail(f"line {step.line}: {step.text}: not reached within {step.within} frames "
                          f"(started at {active.start}, now {frame})")
            return False
        if step.kind == "shot":
            if frame >= active.due:
                self.done(frame, target.shot(step.name))
                return True
            return False
        # press
        if active.hit is not None and (active.applied == 0 or active.released is not None
                                       and frame >= active.released):
            # Met before the first press, while waiting to press again, or by the last press.
            if active.tries == 0:
                self.done(active.hit, f"{what} held before a press")
            else:
                self.done(max(active.released or 0, active.hit),
                          f"{what} at {active.hit}, {active.tries} {'try' if active.tries == 1 else 'tries'}")
            return True
        if active.released is None or frame < active.released:
            return False
        if step.function is None and step.cond is None:
            self.done(active.released)
            return True
        if frame >= active.released + step.every:
            if active.tries >= step.tries:
                self.fail(f"line {step.line}: {step.text}: {what} not met after {active.tries} presses")
            else:
                active.applied, active.released = 0, None
        return False


def vnc_frame(port: int):
    """One full frame from an RFB server (version 3.3, no authentication, raw encoding) as a PIL
    image. RFB: RFC 6143."""
    from PIL import Image
    with socket.create_connection(("127.0.0.1", port), timeout=10) as s:
        def rx(n: int) -> bytes:
            data = b""
            while len(data) < n:
                chunk = s.recv(n - len(data))
                if not chunk:
                    raise EOFError("VNC connection closed")
                data += chunk
            return data

        rx(12)  # the server's version: 3.3 from Qt
        s.sendall(b"RFB 003.003\n")
        if struct.unpack(">I", rx(4))[0] != 1:
            raise ValueError("VNC server wants authentication")
        s.sendall(b"\x01")  # shared
        width, height = struct.unpack(">HH", rx(4))
        rx(16)  # server pixel format
        rx(struct.unpack(">I", rx(4))[0])  # desktop name
        # SetPixelFormat: 32 bpp, depth 24, little-endian, true colour, 8 bits each at 16, 8, 0.
        s.sendall(struct.pack(">BxxxBBBBHHHBBBxxx", 0, 32, 24, 0, 1, 255, 255, 255, 16, 8, 0))
        s.sendall(struct.pack(">BxHi", 2, 1, 0))  # SetEncodings: raw
        s.sendall(struct.pack(">BBHHHH", 3, 0, 0, 0, width, height))  # full update request
        image = Image.new("RGB", (width, height))
        got = 0
        while got < width * height:
            kind = rx(1)[0]
            if kind == 2:  # bell
                continue
            if kind == 3:  # cut text
                rx(3)
                rx(struct.unpack(">I", rx(4))[0])
                continue
            if kind != 0:
                raise ValueError(f"unexpected VNC message {kind}")
            rx(1)
            for _ in range(struct.unpack(">H", rx(2))[0]):
                x, y, w, h, encoding = struct.unpack(">HHHHi", rx(12))
                if encoding != 0:
                    raise ValueError(f"unexpected VNC encoding {encoding}")
                image.paste(Image.frombytes("RGB", (w, h), rx(w * h * 4), "raw", "BGRX"), (x, y))
                got += w * h
        return image


def crop_screens(image):
    """The two 256x192 DS screens of a melonDS window at 1x scale, one above the other.
    It looks for the menu bar (the first light row); the screens start one pixel below
    its bottom edge. The whole frame when no menu bar is found."""
    from PIL import Image
    px = image.load()
    width, height = image.size

    def light(x: int, y: int, level: int) -> bool:
        return all(c >= level for c in px[x, y])

    for y in range(height):
        for x in range(width - 200):
            if light(x, y, 0xE0) and light(x + 200, y, 0xE0):
                while y < height - 384 and light(x + 100, y, 0xC0):
                    y += 1
                top = y + 1
                if top + 384 > height or x + 256 > width:
                    return image
                both = Image.new("RGB", (256, 384))
                both.paste(image.crop((x, top, x + 256, top + 384)), (0, 0))
                return both
    return image


def hook_error(gdb: GdbClient, hook: int) -> str | None:
    """Why `hook` is not the tail call the key hook needs, or None."""
    insn = gdb.read_u32(hook)
    if insn == HOOK_INSN:
        return None
    return (f"{HOOK[0]}+{HOOK[1]:#x} holds {insn:08x}, not the tail call (bx ip); "
            f"the key hook in emu.py needs updating")


def rom_tag(rom: Path) -> str:
    """orig-AWRE for orig/AWRE.nds; for a ROM outside the checkout, its stem and a hash of its path."""
    path = rom.absolute()
    if path.is_relative_to(ROOT):
        return "-".join(path.relative_to(ROOT).with_suffix("").parts)
    return f"{rom.stem}-{hashlib.sha1(str(rom.resolve()).encode()).hexdigest()[:6]}"


class Runner:
    def __init__(self, script: Path, rom: Path, symbols: Symbols, tag: str | None):
        self.script = script
        self.rom = rom
        self.symbols = symbols
        self.tag = tag or rom_tag(rom)
        self.dir = OUT / script.stem
        self.lines: list[str] = []
        self.armed: set[int] = set()  # function breakpoints set besides the vectors and the hook

    def say(self, text: str) -> None:
        print(text, flush=True)
        self.lines.append(text)

    def run(self, steps: list[Step], frames: int, save: Path | None) -> int:
        hook = self.symbols.functions[HOOK[0]] + HOOK[1]
        engine = Engine(steps, self.symbols.functions, self.symbols.data)
        # A screen to grab for the shots, and for a failure.
        try:
            import PIL  # noqa: F401
            screen = True
        except ImportError:
            if any(s.kind == "shot" for s in steps):
                print("error: shot needs PIL (python3-pil)", file=sys.stderr)
                return 2
            screen = False
        self.dir.mkdir(parents=True, exist_ok=True)
        for old in self.dir.glob(f"{self.tag}-*.png"):
            old.unlink()
        work = self.dir / f"{self.tag}.melonds"
        prepare(self.rom, work, save)
        arm9, arm7, vnc = free_ports(3)
        platform = f"vnc:size={VNC_SIZE}x{VNC_SIZE}:port={vnc}" if screen else "offscreen"
        limit = int(frames / 60 + 120)
        start = time.monotonic()
        self.say(f"{self.script.name} on {self.rom} (symbols: {self.symbols.source}; tag {self.tag})")
        for note in overlay_notes(steps, self.symbols, overlay_ranges(self.rom)):
            self.say("  " + note)
        proc = launch(self.rom, work, config(work / "save", arm9, arm7), platform=platform, limit=limit,
                      log=work / "melonds.log")
        gdb = None
        frame = 0
        status = 1
        diagnosis: list[str] = []
        try:
            try:
                gdb = GdbClient.connect(arm9, timeout=20)
            except OSError:
                engine.fail(f"melonDS did not open the GDB port; see {work}/melonds.log")
                status = 2
            else:
                gdb.halt_reason()
                target = Target(gdb, vnc if screen else None, self.dir / self.tag)
                frame = self.drive(gdb, engine, target, hook, frames, start + limit - 5)
                if engine.failure is not None:
                    diagnosis = self.diagnose(gdb, target, hook)
        except (ProtocolError, OSError, TimeoutError) as e:
            engine.fail(f"GDB connection: {e}")
        finally:
            if gdb is not None:
                gdb.kill()
            stop_melonds(proc)
        for line in engine.log:
            self.say("  " + line)
        for line in diagnosis:
            self.say("  " + line)
        ok = engine.failure is None
        self.say(f"run: {'ok' if ok else 'FAIL'}, {frame} frames in {time.monotonic() - start:.1f} s")
        (self.dir / f"{self.tag}.log").write_text("\n".join(self.lines) + "\n")
        return 0 if ok else status

    def drive(self, gdb: GdbClient, engine: Engine, target: Target, hook: int, frames: int,
              deadline: float) -> int:
        for vector in VECTORS:
            gdb.set_breakpoint(vector)
        gdb.set_breakpoint(hook)
        frame, last_poll = 0, 0
        last_frame, last_change = 0, time.monotonic()
        checked_hook = False
        while True:
            want = engine.watched()
            for addr in want - self.armed:
                gdb.set_breakpoint(addr)
            for addr in self.armed - want:
                gdb.clear_breakpoint(addr)
            self.armed = want
            gdb.resume()
            signal_ = gdb.wait_stop(POLL)
            if signal_ is None:
                signal_ = gdb.interrupt()
            regs = gdb.registers()
            pc = regs["pc"]
            frame = gdb.read_u32(VBLANK_COUNT)
            if signal_ == SIGILL or pc in VECTORS:
                kind, where = describe_exception(pc, regs, signal_)
                engine.fail(f"{kind} at {where:08x} ({self.symbols.name(where)}), frame {frame}")
                return frame
            if pc in self.armed:
                engine.hit(pc, frame)
            engine.advance(frame, target, polled=pc == hook)
            if pc == hook:
                if not checked_hook:
                    if error := hook_error(gdb, hook):
                        engine.fail(error)
                        return frame
                    checked_hook = True
                last_poll = frame
                keys = engine.keys(frame)
                if regs["r1"] != keys:
                    gdb.write_register("r1", keys)
            if engine.finished:
                return frame
            if frame - last_poll > HANG_FRAMES:
                engine.fail(f"hang: no key poll for {HANG_FRAMES} frames (frame {frame}, "
                            f"pc {self.symbols.name(pc)})")
                return frame
            now = time.monotonic()
            if frame != last_frame:
                last_frame, last_change = frame, now
            elif now - last_change > 10:
                engine.fail(f"hang: the VBlank counter stopped at {frame} (pc {self.symbols.name(pc)})")
                return frame
            if frame >= frames or now > deadline:
                step = engine.step()
                limit = f"--frames {frames}" if frame >= frames else "the time limit"
                engine.fail(f"line {step.line}: {step.text}: not done by frame {frame} ({limit})")
                return frame

    def diagnose(self, gdb: GdbClient, target: Target, hook: int) -> list[str]:
        """Where the game is: lets it run a moment with only the vectors armed, then stops it.
        The pc where a step failed would mostly be the key hook."""
        out = []
        try:
            for addr in self.armed | {hook}:
                gdb.clear_breakpoint(addr)
            self.armed = set()
            gdb.resume()
            time.sleep(0.1)
            gdb.interrupt()
            regs = gdb.registers()
            out.append(f"pc {regs['pc']:08x} ({self.symbols.name(regs['pc'])}), "
                       f"lr {regs['lr']:08x} ({self.symbols.name(regs['lr'])}), "
                       f"frame {gdb.read_u32(VBLANK_COUNT)}")
        except (ProtocolError, OSError, TimeoutError) as e:
            out.append(f"no registers: {e}")
        if target.port is not None:
            out.append("screen: " + target.shot("fail"))
        return out


# Recording

class Recorder:
    """Writes a recording: a line per change of the keys held, marker comments in between."""

    def __init__(self, out, markers: dict[int, str]):
        self.out = out
        self.markers = markers
        self.keys = 0
        self.frame = 0
        self.seen: set[int] = set()  # markers written since the last key change
        self.last: int | None = None  # the marker written last

    def write(self, text: str) -> None:
        self.out.write(text + "\n")
        self.out.flush()

    def armed(self) -> set[int]:
        return set(self.markers) - self.seen

    def poll(self, frame: int, keys: int) -> None:
        self.frame = frame
        keys &= KEY_MASK
        if keys != self.keys:
            self.keys = keys
            self.seen = set()
            self.write(f"{frame} {format_keys(keys)}")

    def marker(self, frame: int, addr: int) -> None:
        """Writes a marker's first hit after a key change, unless it was also the last one
        written: functions that run every frame would otherwise follow every key line."""
        if addr in self.markers and addr not in self.seen:
            self.seen.add(addr)
            if addr != self.last:
                self.last = addr
                self.write(f"# @ {self.markers[addr]} (frame {frame})")

    def finish(self) -> None:
        self.write(f"{self.frame} end")


def visible_config(work: Path, arm9: int, arm7: int, gdb: bool) -> str:
    return config(work / "save", arm9, arm7, limit_fps=True, gdb=gdb, sound=True, bindings=user_bindings())


def record(out: Path, rom: Path, symbols: Symbols, save: Path | None) -> int:
    hook = symbols.functions[HOOK[0]] + HOOK[1]
    markers = {symbols.functions[m]: m for m in RECORD_MARKERS if m in symbols.functions}
    missing = [m for m in RECORD_MARKERS if m not in symbols.functions]
    if missing:
        print(f"note: no {', '.join(missing)} in {symbols.source}; not marked", file=sys.stderr)
    work = OUT / "record"
    prepare(rom, work, save)
    arm9, arm7 = free_ports(2)
    with open(out, "w") as f:
        rec = Recorder(f, markers)
        rec.write(f"# emu.py record: {rom.name}, sha1 {hashlib.sha1(rom.read_bytes()).hexdigest()}, "
                  f"{'save ' + save.name if save else 'empty save'}")
        rec.write("# VBLANK KEYS: keys held from that VBlank on. Replay: tools/emu.py run THIS_FILE ROM")
        # melonDS logs every GDB packet, about 2.4 MB a minute here: its output goes to /dev/null.
        proc = launch(rom, work, visible_config(work, arm9, arm7, gdb=True), platform=None)
        gdb = None
        try:
            try:
                gdb = GdbClient.connect(arm9, timeout=20)
                gdb.halt_reason()
                if error := hook_error(gdb, hook):
                    print(f"error: {error}", file=sys.stderr)
                    return 2
            except (ProtocolError, OSError, TimeoutError) as e:
                print(f"error: no GDB connection to melonDS: {e}", file=sys.stderr)
                return 2
            gdb.set_breakpoint(hook)
            armed: set[int] = set()
            print(f"recording to {out}; close the melonDS window to stop", flush=True)
            while True:
                want = rec.armed()
                for addr in want - armed:
                    gdb.set_breakpoint(addr)
                for addr in armed - want:
                    gdb.clear_breakpoint(addr)
                armed = want
                gdb.resume()
                gdb.wait_stop(None)
                regs = gdb.registers()
                frame = gdb.read_u32(VBLANK_COUNT)
                if regs["pc"] == hook:
                    rec.poll(frame, regs["r1"])
                else:
                    rec.marker(frame, regs["pc"])
        except (ProtocolError, OSError, TimeoutError, KeyboardInterrupt):
            pass  # the window closed, or Ctrl-C
        finally:
            rec.finish()
            if gdb is not None:
                gdb.kill()
            stop_melonds(proc)
    print(f"recorded {rec.frame} frames to {out}")
    return 0


def play(rom: Path, save: Path | None) -> int:
    work = OUT / "play"
    prepare(rom, work, save)
    proc = launch(rom, work, visible_config(work, 0, 0, gdb=False), platform=None)
    try:
        return proc.wait()
    except KeyboardInterrupt:
        return 130
    finally:
        stop_melonds(proc)
        print(f"save: {work / 'save' / (rom.resolve().stem + '.sav')}")


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    sub = ap.add_subparsers(dest="mode", required=True)
    p = sub.add_parser("run", help="replay a script and check its expect lines")
    p.add_argument("--frames", type=int, default=30000, help="VBlanks before the run fails (60 per second)")
    p.add_argument("--tag", help="name of this ROM's output files (default: from the ROM's path)")
    p.add_argument("script", type=Path)
    p = sub.add_parser("record", help="play in a window and write what you press to OUT")
    p.add_argument("out", type=Path)
    sub.add_parser("play", help="play in a window")
    for p in sub.choices.values():
        p.add_argument("--save", type=Path, help="start from a copy of this save file instead of none")
        p.add_argument("rom", nargs="?", default=DEFAULT_ROM, metavar="ROM[=ELF]")
    args = ap.parse_args()
    if not os.access(MELONDS, os.X_OK):
        print(f"error: melonDS not found at {MELONDS} (set MELONDS; docs/workflow.md)", file=sys.stderr)
        return 2
    rom, elf = parse_rom_arg(args.rom)
    if args.rom == DEFAULT_ROM:
        rom, elf = ROOT / rom, ROOT / elf
    for path in (rom, elf, args.save):
        if path is not None and not path.exists():
            print(f"error: {path} not found", file=sys.stderr)
            return 2
    # SIGTERM unwinds like Ctrl-C, so that the finally blocks stop melonDS.
    signal.signal(signal.SIGTERM, lambda *_: sys.exit(143))
    if args.mode == "play":
        return play(rom, args.save)
    try:
        symbols = Symbols.for_rom(rom, elf)
    except SystemExit as e:
        print(e, file=sys.stderr)
        return 2
    if HOOK[0] not in symbols.functions:
        print(f"error: no {HOOK[0]} (the key hook) in {symbols.source}", file=sys.stderr)
        return 2
    if args.mode == "record":
        return record(args.out, rom, symbols, args.save)
    try:
        steps = parse_script(args.script.read_text())
    except OSError as e:
        print(f"error: {e}", file=sys.stderr)
        return 2
    except ScriptError as e:
        print(f"{args.script}: {e}", file=sys.stderr)
        return 2
    if errors := check_names(steps, symbols):
        print(f"{args.script}:\n  " + "\n  ".join(errors), file=sys.stderr)
        return 2
    try:
        return Runner(args.script, rom, symbols, args.tag).run(steps, args.frames, args.save)
    except KeyboardInterrupt:
        return 130


if __name__ == "__main__":
    sys.exit(main())
