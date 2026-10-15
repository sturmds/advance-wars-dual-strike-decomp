#!/usr/bin/env python3
"""Check tools/emu.py without an emulator or a ROM.

    python3 tools/tests/test_emu.py

Script parsing and its errors, the names check, the overlay code checks
on a small fake ROM and their notes, the step engine and the run loop
against a fake game behind a fake GDB client (fresh hits, retries, relative
and absolute timing, frames without a key poll, memory conditions,
failures, hits in another overlay's code, --shots), the cleanup of old
PNGs, the recording writer and its markers, and the key bindings read from
a sample melonDS.toml.
"""

import contextlib
import io
import struct
import subprocess
import sys
import tempfile
import tomllib
from pathlib import Path

TOOLS = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(TOOLS))

import emu  # noqa: E402
import melonds  # noqa: E402

errors = []


def expect(what, got, want):
    if got != want:
        errors.append(f"{what}: want {want!r}, got {got!r}")


def parse_error(text):
    try:
        emu.parse_script(text)
    except emu.ScriptError as e:
        return str(e)
    return None


# Keys

expect("parse keys", melonds.parse_keys("a+START"), 0x9)
expect("parse no keys", melonds.parse_keys("-"), 0)
expect("format keys", melonds.format_keys(0x9), "a+start")
expect("format no keys", melonds.format_keys(0), "-")


# Parsing

steps = emu.parse_script("""
# comment
2760 start            # recording form
2764 -
+60 hold a+b
@ Title_Run
@ Menu_Open within 30
+5 press a
press down for 2 until Menu_HandleKeys tries 3 every 8
press right until mem gCursor+0x2 s16&0xff == 5
@ mem gState u16 >= 0x10
expect reach Battle_Start within 600
expect mem gState+4 u8 != 0
shot turn1
end
""")
expect("step count", len(steps), 13)
s = steps[0]
expect("absolute", (s.kind, s.at, s.delay, s.keys, s.line), ("hold", 2760, 0, 0x8, 3))
expect("release", (steps[1].kind, steps[1].at, steps[1].keys), ("hold", 2764, 0))
expect("relative hold", (steps[2].kind, steps[2].at, steps[2].delay, steps[2].keys), ("hold", None, 60, 0x3))
expect("wait", (steps[3].kind, steps[3].function, steps[3].within), ("wait", "Title_Run", None))
expect("wait within", steps[4].within, 30)
s = steps[5]
expect("press", (s.kind, s.delay, s.keys, s.hold, s.function, s.tries, s.every), ("press", 5, 1, 4, None, 10, 30))
s = steps[6]
expect("press options", (s.hold, s.function, s.tries, s.every), (2, "Menu_HandleKeys", 3, 8))
expect("press until mem", steps[7].cond, emu.Cond("gCursor", 2, "s16", 0xFF, "==", 5))
expect("wait mem", (steps[8].kind, steps[8].cond), ("wait", emu.Cond("gState", 0, "u16", None, ">=", 16)))
expect("reach", (steps[9].kind, steps[9].function, steps[9].within), ("reach", "Battle_Start", 600))
expect("expect mem", (steps[10].kind, steps[10].cond), ("expect", emu.Cond("gState", 4, "u8", None, "!=", 0)))
expect("shot", (steps[11].kind, steps[11].name), ("shot", "turn1"))
expect("end", steps[12].kind, "end")

for text, part in (("+5", "a time with no action"),
                   ("press q", "unknown key 'q'"),
                   ("press -", "press needs at least one key"),
                   ("press a tries 3", "tries and every need until"),
                   ("press a until", "expected a FUNCTION or mem"),
                   ("press a for 0", "at least 1"),
                   ("expect mem gX u64 == 1", "unknown type 'u64'"),
                   ("expect mem gX u8 =< 1", "unknown comparison '=<'"),
                   ("expect mem gX u8 == one", "value 'one' is not a number"),
                   ("expect reach F", "expect reach FUNCTION within N"),
                   ("@ F within", "within needs a number"),
                   ("shot a/b", "shot NAME"),
                   ("end now", "end takes no arguments"),
                   ("hold a b", "one KEYS argument")):
    message = parse_error(text)
    expect(f"error for {text!r}", message is not None and part in message, True)
expect("errors name every line", parse_error("press q\n\nend now").count("line "), 2)

symbols = melonds.Symbols({"Title_Run": 0x022AD600, "Menu_Open": 0x02000200}, {"gState": 0x02100000},
                          "test", {"Title_Run": "ov004", "Menu_Open": "arm9"})
expect("names", emu.check_names(emu.parse_script(
    "@ Title_Run\n@ Nope\nexpect mem Menu_Open u8 == 0\nexpect mem gNope u8 == 0\nexpect mem gState u8 == 0"),
    symbols), ["line 2: no function Nope in test", "line 3: data symbol Menu_Open: a function, not data",
               "line 4: data symbol gNope: not in test"])
expect("overlay notes", emu.overlay_notes(emu.parse_script("@ Title_Run\n@ Menu_Open"), symbols,
                                          {"Title_Run": ["ov000"]}),
       ["note: line 1: Title_Run is in ov004; ov000 also load at 022ad600, so a hit may be their code"])


# Overlay code checks, on a ROM with only a header, an overlay table and a FAT: ov000 and ov004
# share 0x02300000 (with the same bytes at +0x40), ov001 and ov002 share 0x02500000 and ov002 is
# stored compressed.

def fake_rom(path: Path) -> None:
    files = {0: bytes(range(64)) * 2, 4: bytes(range(64, 128)) + bytes(range(64)), 1: b"\x11" * 64,
             2: b"\x22" * 64}
    table = [(0, 0x02300000, 0), (4, 0x02300000, 4), (1, 0x02500000, 1), (2, 0x02500000, 2)]
    data = bytearray(0x200)
    struct.pack_into("<II", data, 0x48, 0x100, 8 * len(files))
    struct.pack_into("<II", data, 0x50, 0x140, 32 * len(table))
    for i, (id_, ram, file_id) in enumerate(table):
        struct.pack_into("<8I", data, 0x140 + 32 * i, id_, ram, len(files[file_id]), 0, 0, 0, file_id,
                         0x01000000 if id_ == 2 else 0)
    for file_id in sorted(files):
        struct.pack_into("<II", data, 0x100 + 8 * file_id, len(data), len(data) + len(files[file_id]))
        data += files[file_id]
    path.write_bytes(bytes(data))


with tempfile.TemporaryDirectory() as tmp:
    rom = Path(tmp) / "t.nds"
    fake_rom(rom)
    syms = melonds.Symbols(
        {"Own": 0x02300000, "Same": 0x02300040, "Next": 0x02500000, "Packed": 0x02500010, "Main": 0x02000000},
        {}, "test", {"Own": "ov004", "Same": "ov004", "Next": "ov001", "Packed": "ov002", "Main": "arm9"})
    checks, unclear = melonds.overlay_checks(rom, syms, ["Own", "Same", "Next", "Packed", "Main"])
    expect("overlay checks", checks, {"Own": bytes(range(64, 96))})
    expect("overlay checks unclear", unclear, {"Same": ["ov000"], "Next": ["ov002"], "Packed": ["ov001"]})


class FakeMemory:
    def __init__(self, mem):
        self.mem = mem

    def read(self, addr, length):
        return self.mem.get(addr, bytes(length))[:length]


expect("loaded: main", emu.loaded(FakeMemory({}), 0x02000000, {}), True)
expect("loaded: its overlay", emu.loaded(FakeMemory({0x02300000: b"abcd"}), 0x02300000, {0x02300000: b"abcd"}),
       True)
expect("loaded: another overlay", emu.loaded(FakeMemory({0x02300000: b"abce"}), 0x02300000,
                                             {0x02300000: b"abcd"}), False)


# A fake game behind a fake GDB client

HOOK = 0x0200A4A0
TITLE, MENU, STATE = 0x02000100, 0x02000200, 0x02000300
CURSOR = 0x02100000
SKIPPED = range(60, 65)  # frames without a key poll, as while loading


class FakeGame:
    """Title runs every frame from 10 to 99. Menu runs on the frame after the first frame A is
    held, from frame 40 on (earlier presses are dropped). State runs every frame from 100.
    Each new press of right adds 1 to the u16 at CURSOR."""

    def __init__(self, frames: int = 400, code_from: int = 0, title_calls: int = 1):
        self.code_from = code_from  # Title's overlay is loaded from this frame on
        self.title_calls = title_calls  # calls of Title's address a frame
        self.title_stops = 0
        self.breakpoints: set[int] = set()
        self.keys: dict[int, int] = {}
        self.cursor = 0
        self.frame = 0
        self.r1 = 0
        self.stops = self.run(frames)
        self.pc = 0
        self.resumes = 0

    def run(self, frames: int):
        menu_at, prev = None, 0
        for frame in range(1, frames):
            self.frame = frame
            calls = []
            if 10 <= frame < 100:
                calls += [TITLE] * self.title_calls
            if frame == menu_at:
                calls.append(MENU)
            if frame >= 100:
                calls.append(STATE)
            for addr in calls:
                if addr in self.breakpoints:
                    yield addr
            if frame in SKIPPED:
                continue
            self.r1 = 0
            if HOOK in self.breakpoints:
                yield HOOK
            keys = self.keys[frame] = self.r1
            if keys & 1 and not prev & 1 and frame >= 40 and menu_at is None:
                menu_at = frame + 1
            if keys & 0x10 and not prev & 0x10:
                self.cursor += 1
            prev = keys

    # The GdbClient methods the runner uses
    def set_breakpoint(self, addr):
        self.breakpoints.add(addr)

    def clear_breakpoint(self, addr):
        self.breakpoints.discard(addr)

    def resume(self):
        self.resumes += 1
        self.pc = next(self.stops)
        self.title_stops += self.pc == TITLE

    def wait_stop(self, timeout):
        return 5

    def interrupt(self, timeout=5):
        return 2

    def registers(self):
        return {"pc": self.pc, "r1": self.r1, "lr": 0, "cpsr": 0x1F}

    def write_register(self, name, value):
        assert name == "r1"
        self.r1 = value

    def read(self, addr, length):
        if addr == CURSOR:
            return struct.pack("<H", self.cursor)[:length]
        if addr == HOOK:
            return struct.pack("<I", emu.HOOK_INSN)
        if addr == TITLE and self.frame >= self.code_from:
            return b"T" * length
        return bytes(length)

    def read_u32(self, addr):
        if addr == melonds.VBLANK_COUNT:
            return self.frame
        return int.from_bytes(self.read(addr, 4), "little")


class Shots:
    def __init__(self, game):
        self.game = game
        self.taken = []

    def read(self, addr, length):
        return self.game.read(addr, length)

    def shot(self, name):
        self.taken.append((name, self.game.frame))
        return f"{name}.png"


def drive(script: str, frames: int = 400, code_from: int = 0, shots: int = 0, title_calls: int = 1):
    game = FakeGame(frames + 10, code_from, title_calls)
    syms = melonds.Symbols({"Keypad_Poll": HOOK - 0x30, "Title_Run": TITLE, "Menu_Open": MENU,
                            "State_Run": STATE}, {"gCursor": CURSOR}, "test")
    runner = emu.Runner(Path("t.inp"), Path("t.nds"), syms, "t", shots)
    runner.checks = {TITLE: b"TTTT"}
    steps = emu.parse_script(script)
    assert not emu.check_names(steps, syms), emu.check_names(steps, syms)
    engine = emu.Engine(steps, syms.functions, syms.data)
    target = Shots(game)
    end = runner.drive(game, engine, target, HOOK, frames, deadline=float("inf"))
    return engine, game, target, end


def held(game, mask):
    return [f for f, k in sorted(game.keys.items()) if k & mask]


engine, game, shots, end = drive("""
@ Title_Run
+5 press a until Menu_Open every 10
@ State_Run within 200
expect reach State_Run within 5
press right until mem gCursor u16 == 3 every 2
expect mem gCursor u16 == 3
+10 shot here
end
""")
expect("run passes", engine.failure, None)
log = engine.log
expect("fresh hit of Title", log[0], "    10  @ Title_Run  (after 9 frames)")
# Presses at 15, then every 10 frames after each release; frame 40 is the first that counts.
a_frames = held(game, 1)
expect("first press 5 frames after the anchor", a_frames[:4], [15, 16, 17, 18])
expect("retries until the menu opens", a_frames, [15, 16, 17, 18, 29, 30, 31, 32, 43, 44, 45, 46])
expect("press log", log[1], "    47  +5 press a until Menu_Open every 10  (Menu_Open at 44, 3 tries)")
expect("wait for State", log[2], "   100  @ State_Run within 200  (after 53 frames)")
expect("reach is a fresh hit", log[3], "   101  expect reach State_Run within 5  (after 1 frames)")
expect("cursor taps", [f for f in held(game, 0x10)], [101, 102, 103, 104, 107, 108, 109, 110, 113, 114, 115, 116])
expect("until mem", log[4], "   117  press right until mem gCursor u16 == 3 every 2  (the condition at 114, 3 tries)")
expect("expect mem", log[5], "   117  expect mem gCursor u16 == 3  (0x3)")
expect("shot after the delay", shots.taken, [("here", 117 + 10 + emu.SHOT_DELAY)])
expect("end", (engine.finished, end, log[-1]), (True, 133, "   133  end"))

# Recording form: absolute frames, polls skipped while loading, presses counted in polled frames.
engine, game, _, _ = drive("""
30 a
32 -
61 b
67 -
end
""")
expect("absolute holds", held(game, 1), [30, 31])
expect("hold from the first poll at or after its frame", held(game, 2), [65, 66])  # 60-64 not polled
engine, game, _, _ = drive("58 press x for 4\nend")
expect("press frames are polled frames", held(game, 0x400), [58, 59, 65, 66])
# A press starts after a polled frame with its keys up: after a press of the same key, and on a
# retry whose release fell in frames without a key poll (60-64).
engine, game, _, _ = drive("@ Title_Run\npress a\npress a\nend")
expect("same key twice", held(game, 1), [10, 11, 12, 13, 15, 16, 17, 18])
engine, game, _, _ = drive("56 press b until Menu_Open every 0 tries 2")
expect("retry after unpolled frames", held(game, 2), [56, 57, 58, 59, 66, 67, 68, 69])

# Failures
engine, *_ = drive("@ Title_Run\npress a until Menu_Open tries 2 every 5")
expect("tries run out", engine.failure,
       "line 2: press a until Menu_Open tries 2 every 5: Menu_Open not met after 2 presses")
engine, *_ = drive("expect reach State_Run within 50")
expect("reach too late", engine.failure,
       "line 1: expect reach State_Run within 50: not reached within 50 frames (started at 1, now 52)")
engine, *_ = drive("@ Title_Run\nexpect mem gCursor u16 == 1")
expect("expect mem fails", engine.failure, "line 2: expect mem gCursor u16 == 1: read 0x0 (0)")
engine, _, _, end = drive("@ Menu_Open", frames=150)
expect("frame limit", engine.failure, "line 1: @ Menu_Open: not done by frame 150 (--frames 150)")
# An until already met before the press: no press.
engine, game, *_ = drive("@ State_Run\npress right until mem gCursor u16 == 0\nend")
expect("met before a press", (held(game, 0x10), engine.log[1].endswith("(the condition held before a press)")),
       ([], True))
# Title's breakpoint stops in another overlay's code until frame 20: those hits do not count.
engine, game, *_ = drive("@ Title_Run\nend", code_from=20)
expect("hit in its own code", (engine.log[0], game.title_stops), ("    20  @ Title_Run  (after 19 frames)", 11))
# Called 20 times a frame there, it stops FOREIGN_STOPS times a frame.
engine, game, *_ = drive("@ Title_Run\nend", code_from=20, title_calls=20)
expect("stops in other code", (engine.log[0], game.title_stops),
       ("    20  @ Title_Run  (after 19 frames)", 10 * emu.FOREIGN_STOPS + 1))
# --shots: at the first stop at or after each multiple of N; 60 to 64 have no stop.
engine, game, shots, _ = drive("@ State_Run\nend", shots=31)
expect("shots", shots.taken, [("f00031", 31), ("f00065", 65), ("f00093", 93)])
expect("shot names", (emu.shot_name(7), emu.shot_name(123456)), ("f00007", "f123456"))
# A wait does not count hits from before it started: Title ran in frame 30 before the
# key poll where the wait started.
engine, *_ = drive("@ Title_Run\n+20 hold -\n@ Title_Run\nend")
expect("fresh hit after the previous step", engine.log[2], "    31  @ Title_Run  (after 1 frames)")


# Old screenshots: those of the tag, not those of a longer tag that starts with it.

with tempfile.TemporaryDirectory() as tmp:
    d = Path(tmp)
    for name in ("build-AWRE.log", "build-AWRE-turn1.png", "build-AWRE-fail.png", "build-AWRE-x.melonds",
                 "build-AWRE-x-turn1.png", "build-AWRE-x.log", "build-AWRE-xy-turn1.png", "orig-AWRE-turn1.png"):
        (d / name).touch()
    expect("old PNGs", [p.name for p in emu.old_pngs(d, "build-AWRE")],
           ["build-AWRE-fail.png", "build-AWRE-turn1.png", "build-AWRE-xy-turn1.png"])
    expect("old PNGs of the longer tag", [p.name for p in emu.old_pngs(d, "build-AWRE-x")],
           ["build-AWRE-x-turn1.png"])


# Recording

expect("markers are in config/AWRE/arm9",
       [m.name for m in emu.RECORD_MARKERS if m.name not in melonds.config_symbol_table()[0][melonds.STT_FUNC]], [])
out = io.StringIO()
rec = emu.Recorder(out, {MENU: emu.Marker("Menu_Open", "menu"), STATE: emu.Marker("State_Run", "state", True),
                         TITLE: emu.Marker("Title_Run", "title", True)})
rec.poll(10, 0)
rec.marker(11, STATE)
expect("a marker that ran is off", rec.armed(), {MENU, TITLE})
for _ in range(emu.FOREIGN_STOPS - 1):
    rec.marker(12, MENU, is_loaded=False)  # another overlay's code: not written
expect("on after a few stops in other code", rec.armed(), {MENU, TITLE})
rec.marker(12, MENU, is_loaded=False)
expect("then off until the key poll", rec.armed(), {TITLE})
rec.poll(13, 0)
rec.marker(14, MENU)
expect("a marker that runs once turns the others on again", rec.armed(), {STATE, TITLE})
rec.marker(15, STATE)  # every frame, and the last such written: not written again
rec.poll(20, 0x2008)  # the debug bit is dropped
rec.marker(21, TITLE)
rec.marker(22, STATE)  # every frame, but another one was written since
rec.poll(23, 0x8)
rec.poll(24, 0)
rec.marker(25, MENU)
rec.finish()
expect("recording", out.getvalue().splitlines(),
       ["# @ State_Run (frame 11): state", "# @ Menu_Open (frame 14): menu", "20 start",
        "# @ Title_Run (frame 21): title", "# @ State_Run (frame 22): state", "24 -",
        "# @ Menu_Open (frame 25): menu", "24 end"])
replay = emu.parse_script(out.getvalue())
expect("a recording parses", [(s.at, s.kind, s.keys) for s in replay],
       [(20, "hold", 8), (24, "hold", 0), (24, "end", 0)])

# Markers of a ROM: one missing from the symbols and one whose hits cannot be told apart are
# left out, with a note.
with tempfile.TemporaryDirectory() as tmp:
    rom = Path(tmp) / "t.nds"
    fake_rom(rom)
    syms = melonds.Symbols({"Own": 0x02300000, "Same": 0x02300040, "Main": 0x02000000}, {}, "test",
                           {"Own": "ov004", "Same": "ov004", "Main": "arm9"})
    saved, emu.RECORD_MARKERS = emu.RECORD_MARKERS, tuple(emu.Marker(n, n) for n in ("Own", "Same", "Main", "Gone"))
    notes = io.StringIO()
    with contextlib.redirect_stderr(notes):
        markers, code = emu.record_markers(rom, syms)
    emu.RECORD_MARKERS = saved
    expect("record markers", ({m.name for m in markers.values()}, code), ({"Own", "Main"},
                                                                         {0x02300000: bytes(range(64, 96))}))
    expect("record marker notes", notes.getvalue().splitlines(),
           ["note: no Gone in test; not marked", "note: Same cannot be told apart from ov000 at its address; "
            "not marked"])


# quit_melonds: under `timeout`, as launch starts it, the program gets one SIGTERM and has time
# to finish; a second one would kill melonDS before it writes the save.
CHILD = """
import signal, sys, time
got = []
signal.signal(signal.SIGTERM, lambda *_: got.append(1))
print("up", flush=True)
while not got:
    time.sleep(0.01)
time.sleep(0.3)
open(sys.argv[1], "w").write(str(len(got)))
"""
with tempfile.TemporaryDirectory() as tmp:
    done = Path(tmp) / "done"
    proc = subprocess.Popen(["timeout", "-k", "5", "30", sys.executable, "-c", CHILD, str(done)],
                            stdout=subprocess.PIPE, start_new_session=True)
    proc.stdout.readline()
    expect("quit_melonds", (melonds.quit_melonds(proc), done.exists() and done.read_text()), (True, "1"))
    proc.stdout.close()


# Key bindings and the config

with tempfile.TemporaryDirectory() as tmp:
    (Path(tmp) / "melonDS").mkdir()
    (Path(tmp) / "melonDS/melonDS.toml").write_text("""\
LimitFPS = true

[Instance0]
JoystickID = 1
SaveFilePath = "/somewhere/saves"

[Instance0.Firmware]
Username = "Someone"
BirthdayMonth = 4

[Instance0.Keyboard]
A = 90
B = 88
HK_Pause = -1
"Odd key" = 1
Name = "x"

[Instance0.Joystick]
A = 1
Up = 257
""")
    bindings = melonds.user_bindings({"XDG_CONFIG_HOME": tmp})
    expect("bindings", bindings, {"Keyboard": {"A": 90, "B": 88, "HK_Pause": -1}, "Joystick": {"A": 1, "Up": 257},
                                  "JoystickID": 1})
    expect("no config, no bindings", melonds.user_bindings({"XDG_CONFIG_HOME": tmp + "/none"}), {})
    text = melonds.config(Path("/w/save"), 1, 2, limit_fps=True, sound=True, bindings=bindings)
    parsed = tomllib.loads(text)
    expect("config bindings", (parsed["Instance0"]["Keyboard"], parsed["Instance0"]["JoystickID"]),
           ({"A": 90, "B": 88, "HK_Pause": -1}, 1))
    expect("config keeps no firmware", ("Firmware" in parsed["Instance0"], "Someone" in text), (False, False))
    expect("config settings", (parsed["LimitFPS"], parsed["Instance0"]["Audio"]["Volume"],
                               parsed["Instance0"]["SaveFilePath"]), (True, 256, "/w/save"))
    plain = tomllib.loads(melonds.config(Path("/w/save"), 1, 2))
    expect("melonds_check config", (plain["LimitFPS"], plain["Instance0"]["Audio"]["Volume"],
                                    plain["Instance0"]["Gdb"]["Enabled"], "Keyboard" in plain["Instance0"]),
           (False, 0, True, False))


if errors:
    sys.exit("test_emu: " + "\n  ".join(errors))
print("test_emu: ok")
