"""melonDS launch, config, ELF symbols and process control, shared by tools/melonds_check.py and tools/emu.py.

Each run gets a directory of its own (config, home and save directory), so
the user's melonDS settings and home directory are neither read nor written.
The one exception is `user_bindings`, which reads the user's key and
joystick bindings for the visible modes of emu.py. melonDS comes from env
MELONDS, default melonDS/build/melonDS beside the repo's main checkout.

Facts about melonDS (config keys, exception entry, GDB stub):
docs/third-party/melonds.md.
"""

import bisect
import hashlib
import os
import re
import shutil
import signal
import socket
import struct
import subprocess
import tomllib
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent


def default_melonds() -> str:
    """melonDS built in a melonDS checkout beside the main checkout of this repo (worktrees included)."""
    try:
        common = subprocess.run(["git", "rev-parse", "--path-format=absolute", "--git-common-dir"], cwd=ROOT,
                                capture_output=True, text=True, check=True).stdout.strip()
        main = Path(common).parent
    except (OSError, subprocess.CalledProcessError):
        main = ROOT
    return str(main.parent / "melonDS/build/melonDS")


MELONDS = os.environ.get("MELONDS") or default_melonds()

VBLANK_COUNT = 0x027FFC3C  # SHARED_FRAME_COUNTER (include/sdk/os_common.h), raised by the VBlank interrupt
VECTORS = {0xFFFF0004: "undefined instruction", 0xFFFF000C: "prefetch abort",
           0xFFFF0010: "data abort"}
SIGILL = 4
# Key bits as the game reads them (include/game/keypad.h).
KEYS = {"a": 0x1, "b": 0x2, "select": 0x4, "start": 0x8, "right": 0x10, "left": 0x20, "up": 0x40,
        "down": 0x80, "r": 0x100, "l": 0x200, "x": 0x400, "y": 0x800}


def parse_keys(text: str) -> int:
    """`a+start` to a key mask; `-` is no key. Raises ValueError on an unknown key."""
    if text == "-":
        return 0
    mask = 0
    for key in text.lower().split("+"):
        if key not in KEYS:
            raise ValueError(f"unknown key {key!r} (keys: {', '.join(KEYS)}, or - for none)")
        mask |= KEYS[key]
    return mask


def format_keys(mask: int) -> str:
    """The inverse of parse_keys, keys in KEYS order."""
    return "+".join(k for k, bit in KEYS.items() if mask & bit) or "-"


# Symbols

STT_OBJECT, STT_FUNC = 1, 2


def elf_symbol_table(path: Path) -> tuple[dict[int, dict[str, int]], dict[str, str]]:
    """Symbols of a 32-bit little-endian ELF by type (STT_FUNC, STT_OBJECT): name to address,
    Thumb bit cleared for functions. Of two symbols with one name, the first is kept. Also the
    module of each function: its section's name in lower case (arm9, itcm, ov004)."""
    data = path.read_bytes()
    if data[:4] != b"\x7fELF" or data[4] != 1 or data[5] != 1:
        raise SystemExit(f"error: {path}: not a 32-bit little-endian ELF")
    shoff, = struct.unpack_from("<I", data, 0x20)
    shentsize, shnum = struct.unpack_from("<HH", data, 0x2E)
    sections = [struct.unpack_from("<IIIIIIIIII", data, shoff + i * shentsize) for i in range(shnum)]
    out: dict[int, dict[str, int]] = {STT_FUNC: {}, STT_OBJECT: {}}
    modules: dict[str, str] = {}
    shstrndx, = struct.unpack_from("<H", data, 0x32)
    names_at = sections[shstrndx][4]

    def section_name(index: int) -> str:
        if not 0 < index < len(sections):
            return ""
        start = names_at + sections[index][0]
        return data[start:data.index(b"\0", start)].decode().lower()

    for sh in sections:
        if sh[1] != 2:  # SHT_SYMTAB
            continue
        strtab = sections[sh[6]]
        for off in range(sh[4], sh[4] + sh[5], 16):
            name_off, value, _size, info, _other, shndx = struct.unpack_from("<IIIBBH", data, off)
            kind = info & 0xF
            if kind not in out:
                continue
            start = strtab[4] + name_off
            name = data[start:data.index(b"\0", start)].decode()
            if name not in out[kind]:
                out[kind][name] = value & ~1 if kind == STT_FUNC else value
                if kind == STT_FUNC:
                    modules[name] = section_name(shndx)
    return out, modules


def config_symbol_table() -> tuple[dict[int, dict[str, int]], dict[str, str]]:
    """The same as elf_symbol_table, from config/AWRE/arm9/ (kind function; kind data or bss).
    A function's module is its symbols.txt's directory (arm9, itcm, ov004)."""
    out: dict[int, dict[str, int]] = {STT_FUNC: {}, STT_OBJECT: {}}
    modules: dict[str, str] = {}
    pattern = re.compile(r"^(\S+) kind:(function|data|bss)\b\S* addr:(0x[0-9a-fA-F]+)")
    base = ROOT / "config/AWRE/arm9"
    for path in sorted(base.rglob("symbols.txt")):
        module = path.parent.name
        for line in path.read_text().splitlines():
            m = pattern.match(line)
            if m:
                kind = STT_FUNC if m[2] == "function" else STT_OBJECT
                if m[1] not in out[kind]:
                    out[kind][m[1]] = int(m[3], 16)
                    if kind == STT_FUNC:
                        modules[m[1]] = module
    return out, modules


def overlay_ranges(rom: Path) -> dict[str, tuple[int, int]]:
    """The RAM range (start, end with .bss) of each ARM9 overlay in the ROM's overlay table, by
    module name (ov000)."""
    data = rom.read_bytes()
    offset, size = struct.unpack_from("<II", data, 0x50)
    out = {}
    for entry in range(offset, offset + size, 32):
        id_, ram, ram_size, bss_size = struct.unpack_from("<IIII", data, entry)
        out[f"ov{id_:03d}"] = (ram, ram + ram_size + bss_size)
    return out


class Symbols:
    """Function and data addresses of one ROM, from its ELF or, for retail, from config/."""

    def __init__(self, functions: dict[str, int], data: dict[str, int], source: str,
                 modules: dict[str, str] | None = None):
        self.functions = functions
        self.data = data
        self.source = source
        self.modules = modules or {}  # function name to module: arm9, itcm, ov004
        self.by_addr = sorted((a, n) for n, a in functions.items())

    @classmethod
    def for_rom(cls, rom: Path, elf: Path | None) -> "Symbols":
        """From ELF, from config/AWRE/arm9/ when the ROM is retail, or from the arm9.elf beside the ROM."""
        retail = (ROOT / "config/AWRE/rom.sha1").read_text().split()[0]
        if elf is None and hashlib.sha1(rom.read_bytes()).hexdigest() == retail:
            (table, modules), source = config_symbol_table(), "config/AWRE/arm9 (retail ROM)"
        else:
            elf = elf or rom.parent / "arm9.elf"
            if not elf.exists():
                raise SystemExit(f"error: no {elf}: a ROM that is not retail needs an ELF beside it or as ROM=ELF")
            (table, modules), source = elf_symbol_table(elf), str(elf)
        return cls(table[STT_FUNC], table[STT_OBJECT], source, modules)

    def name(self, addr: int) -> str:
        """FUNCTION+offset for a code address, or the address in hex when no function is near."""
        i = bisect.bisect_right(self.by_addr, (addr, "\uffff")) - 1
        if i < 0 or addr - self.by_addr[i][0] >= 0x10000:
            return f"{addr:08x}"
        base, name = self.by_addr[i]
        return f"{name}+{addr - base:#x}"


def parse_rom_arg(arg: str) -> tuple[Path, Path | None]:
    rom, _, elf = arg.partition("=")
    return Path(rom), Path(elf) if elf else None


# melonDS config

CONFIG = """\
LimitFPS = {limit_fps}
AudioSync = false
MuteFastForward = true

[Emu]
DirectBoot = true
ExternalBIOSEnable = false
ConsoleType = 0

[JIT]
Enable = false

[Screen]
UseGL = false

[Mic]
InputType = 0

[Instance0]
SaveFilePath = "{save}"
SavestatePath = "{save}"
CheatFilePath = "{save}"
EnableCheats = false
{instance}
[Instance0.Audio]
Volume = {volume}

[Instance0.Window0]
ShowOSD = false

[Instance0.Gdb]
Enabled = {gdb}

[Instance0.Gdb.ARM9]
Port = {arm9}
BreakOnStartup = true

[Instance0.Gdb.ARM7]
Port = {arm7}
BreakOnStartup = false
{tables}"""


def config(save: Path, arm9: int = 0, arm7: int = 0, *, limit_fps: bool = False, gdb: bool = True,
           sound: bool = False, bindings: dict | None = None) -> str:
    """melonDS.toml text. The defaults are melonds_check.py's: no frame limit, no sound, GDB on.
    bindings is what user_bindings returns."""
    instance, tables = "", ""
    if bindings:
        if "JoystickID" in bindings:
            instance = f"JoystickID = {bindings['JoystickID']}\n"
        for table in ("Keyboard", "Joystick"):
            if table in bindings:
                tables += f"\n[Instance0.{table}]\n" + "".join(
                    f"{k} = {v}\n" for k, v in bindings[table].items())
    return CONFIG.format(limit_fps=str(limit_fps).lower(), save=save, instance=instance,
                         volume=256 if sound else 0, gdb=str(gdb).lower(), arm9=arm9, arm7=arm7,
                         tables=tables)


BINDING_KEY = re.compile(r"^[A-Za-z0-9_]+$")


def user_bindings(env: dict[str, str] | None = None) -> dict:
    """The user's key and joystick bindings from their melonDS.toml, read-only:
    `[Instance0.Keyboard]`, `[Instance0.Joystick]` and `Instance0.JoystickID`, integer
    values only. Nothing else is read, the firmware table least of all (it holds the
    owner's name and birthday). Empty when there is no config."""
    env = os.environ if env is None else env
    base = Path(env["XDG_CONFIG_HOME"]) if env.get("XDG_CONFIG_HOME") else Path.home() / ".config"
    try:
        with open(base / "melonDS/melonDS.toml", "rb") as f:
            data = tomllib.load(f)
    except (OSError, tomllib.TOMLDecodeError):
        return {}
    instance = data.get("Instance0")
    if not isinstance(instance, dict):
        return {}
    out: dict = {}
    for table in ("Keyboard", "Joystick"):
        values = instance.get(table)
        if isinstance(values, dict):
            out[table] = {k: v for k, v in values.items()
                          if BINDING_KEY.match(k) and isinstance(v, int) and not isinstance(v, bool)}
    joystick = instance.get("JoystickID")
    if isinstance(joystick, int) and not isinstance(joystick, bool):
        out["JoystickID"] = joystick
    return out


# Processes

def free_ports(n: int) -> list[int]:
    socks = []
    for _ in range(n):
        s = socket.socket()
        s.bind(("127.0.0.1", 0))
        socks.append(s)
    ports = [s.getsockname()[1] for s in socks]
    for s in socks:
        s.close()
    return ports


# Passed on to a visible melonDS, so that it can open a window and play sound.
DISPLAY_ENV = ("DISPLAY", "WAYLAND_DISPLAY", "XDG_RUNTIME_DIR", "XAUTHORITY", "XDG_SESSION_TYPE",
               "DBUS_SESSION_BUS_ADDRESS")


def prepare(rom: Path, work: Path, save: Path | None) -> None:
    """Empties `work` and makes its config, home and save directories, with a copy of `save`."""
    if work.exists():
        shutil.rmtree(work)
    for sub in ("config/melonDS", "home", "save"):
        (work / sub).mkdir(parents=True)
    if save is not None:
        shutil.copyfile(save, work / "save" / (rom.resolve().stem + ".sav"))


def launch(rom: Path, work: Path, config_text: str, *, platform: str | None = "offscreen",
           limit: int | None = None, log: Path | None = None) -> subprocess.Popen:
    """Starts melonDS on `rom` with `config_text` in `work` (made by `prepare`).

    platform is QT_QPA_PLATFORM (offscreen, vnc:...), or None for a window on the
    user's display. limit stops melonDS after that many seconds, in case the caller
    dies. log takes melonDS's output (None: /dev/null). melonDS runs in a session of
    its own so that stop_melonds can stop it and `timeout` together.
    """
    (work / "config/melonDS/melonDS.toml").write_text(config_text)
    env = {
        "PATH": "/usr/bin:/bin",
        "HOME": str(work / "home"),
        "XDG_CONFIG_HOME": str(work / "config"),
        "XDG_CACHE_HOME": str(work / "home/.cache"),
        "XDG_DATA_HOME": str(work / "home/.local/share"),
    }
    if platform is None:
        env.update({k: os.environ[k] for k in DISPLAY_ENV if k in os.environ})
    else:
        env["QT_QPA_PLATFORM"] = platform
        env["SDL_AUDIODRIVER"] = "dummy"
    command = [MELONDS, str(rom.resolve())]
    if limit is not None:
        command = ["timeout", "-k", "5", str(limit)] + command
    out = open(log, "w") if log is not None else open(os.devnull, "w")
    with out:
        return subprocess.Popen(command, env=env, cwd=work, stdout=out, stderr=subprocess.STDOUT,
                                stdin=subprocess.DEVNULL, start_new_session=True)


def start_melonds(rom: Path, work: Path, arm9: int, arm7: int, limit: int, save: Path | None) -> subprocess.Popen:
    """melonds_check.py's launch: no window, no sound, no frame limit, log in work/melonds.log."""
    prepare(rom, work, save)
    # timeout stops melonDS if this script dies; the session lets stop_melonds kill both.
    return launch(rom, work, config(work / "save", arm9, arm7), limit=limit, log=work / "melonds.log")


def stop_melonds(proc: subprocess.Popen) -> None:
    """Stops the process group `launch` started, and only that one."""
    for sig in (signal.SIGTERM, signal.SIGKILL):
        try:
            os.killpg(proc.pid, sig)
        except ProcessLookupError:
            return
        try:
            proc.wait(3)
            return
        except subprocess.TimeoutExpired:
            pass


def describe_exception(pc: int, regs: dict[str, int], signal_: int) -> tuple[str, int]:
    """The kind of an ARM9 exception stop and the faulting address."""
    if signal_ == SIGILL and pc not in VECTORS:
        # melonDS stops before taking the exception; its pc then reads one instruction ahead.
        return "undefined instruction", pc - (2 if regs["cpsr"] & 0x20 else 4)
    kind = VECTORS[pc]
    thumb = regs[{"undefined instruction": "spsr_und"}.get(kind, "spsr_abt")] & 0x20
    if kind == "undefined instruction":
        return kind, regs["lr_und"] - (2 if thumb else 4)
    if kind == "prefetch abort":
        return kind, regs["lr_abt"] - 4
    return kind, regs["lr_abt"] - 8
