"""Read the ARM9 modules, sections, symbols and relocations of the dsd config.

Shared by tools/resolve_overlay_relocs.py and tools/scan_pointers.py. Needs
extract/ (run `make extract` first). No dsd run, no build needed.

A module here is main, itcm, dtcm or one overlay. Its sections come from the
header lines of its delinks.txt (`.text start:... end:... kind:code`), its
bytes from extract/, its symbols from symbols.txt and its relocations from
relocs.txt.
"""

import re
from dataclasses import dataclass, field
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
CONFIG = ROOT / "config/AWRE/arm9"
EXTRACT = ROOT / "extract"

SECTION_RE = re.compile(r"^\s+(\S+)\s+start:(0x[0-9a-f]+) end:(0x[0-9a-f]+) kind:(\w+)")
SYMBOL_RE = re.compile(r"^(\S+) kind:(\S+) addr:(0x[0-9a-f]+)(.*)$")
RELOC_RE = re.compile(r"^from:(0x[0-9a-f]+) kind:(\S+)(?: to:(0x[0-9a-f]+)(?: add:(-?0x[0-9a-f]+))? module:(\S+))?$")


@dataclass
class Section:
    name: str
    start: int
    end: int
    kind: str  # code, rodata, data, bss


@dataclass
class Symbol:
    name: str
    kind: str
    addr: int
    rest: str  # attributes after addr, such as " ambiguous"

    @property
    def ambiguous(self) -> bool:
        return " ambiguous" in self.rest

    @property
    def size(self) -> int | None:
        m = re.search(r"size=(0x[0-9a-f]+)", self.kind)
        return int(m.group(1), 16) if m else None


@dataclass
class Reloc:
    source: int
    kind: str
    to: int
    add: int
    module: str  # main, itcm, dtcm, overlay(N), overlays(N,M,...), none

    def candidates(self) -> list[str]:
        """Module names this relocation may point into."""
        m = re.fullmatch(r"overlays?\(([\d,]+)\)", self.module)
        if m:
            return [f"ov{int(i):03d}" for i in m.group(1).split(",")]
        return [self.module] if self.module != "none" else []

    def line(self) -> str:
        add = ""
        if self.add:
            add = f" add:{self.add:#x}" if self.add > 0 else f" add:-{-self.add:#x}"
        return f"from:{self.source:#010x} kind:{self.kind} to:{self.to:#010x}{add} module:{self.module}"


@dataclass
class Module:
    name: str  # main, itcm, dtcm, ov000...
    dir: Path
    sections: list[Section]
    data: bytes  # initialised bytes, loaded at `start`
    symbols: list[Symbol] = field(default_factory=list)
    relocs: dict[int, Reloc] = field(default_factory=dict)
    # Source files of delinks.txt with each of their ranges: (start, end, path).
    files: list[tuple[int, int, str]] = field(default_factory=list)

    @property
    def start(self) -> int:
        return self.sections[0].start

    @property
    def init_end(self) -> int:
        return self.start + len(self.data)

    @property
    def end(self) -> int:
        return max(s.end for s in self.sections)

    def section_at(self, addr: int) -> Section | None:
        for s in self.sections:
            if s.start <= addr < s.end:
                return s
        return None

    def word(self, addr: int) -> int:
        off = addr - self.start
        return int.from_bytes(self.data[off:off + 4], "little")

    def is_initialised(self, addr: int) -> bool:
        return self.start <= addr < self.init_end


def _sections(delinks: Path) -> list[Section]:
    out = []
    for line in delinks.read_text().splitlines():
        if line and not line[0].isspace():
            break
        m = SECTION_RE.match(line)
        if m:
            out.append(Section(m.group(1), int(m.group(2), 16), int(m.group(3), 16), m.group(4)))
    return out


def read_files(delinks: Path) -> list[tuple[int, int, str]]:
    """The (start, end, source path) ranges of the files in delinks.txt,
    sorted by start."""
    out = []
    path = None
    for line in delinks.read_text().splitlines():
        if line and not line[0].isspace():
            path = line.rstrip(":")
            continue
        m = re.match(r"\s+\S+\s+start:(0x[0-9a-f]+) end:(0x[0-9a-f]+)", line)
        if path and m:
            out.append((int(m.group(1), 16), int(m.group(2), 16), path))
    return sorted(out)


def read_symbols(path: Path) -> list[Symbol]:
    out = []
    for line in path.read_text().splitlines():
        m = SYMBOL_RE.match(line)
        if m:
            out.append(Symbol(m.group(1), m.group(2), int(m.group(3), 16), m.group(4)))
    return out


def read_relocs(path: Path) -> dict[int, Reloc]:
    out = {}
    for line in path.read_text().splitlines():
        m = RELOC_RE.match(line)
        if not m:
            raise ValueError(f"{path}: cannot parse {line!r}")
        add = int(m.group(4), 16) if m.group(4) else 0
        # A link-time constant (`kind:link_time_const(...)`) has no target.
        to = int(m.group(3), 16) if m.group(3) else 0
        r = Reloc(int(m.group(1), 16), m.group(2), to, add, m.group(5) or "none")
        out[r.source] = r
    return out


def load() -> dict[str, Module]:
    if not (EXTRACT / "arm9/arm9.bin").exists():
        raise SystemExit("error: extract/ is missing, run `make extract` first")
    dirs = {"main": CONFIG, "itcm": CONFIG / "itcm", "dtcm": CONFIG / "dtcm"}
    bins = {"main": EXTRACT / "arm9/arm9.bin", "itcm": EXTRACT / "arm9/itcm.bin",
            "dtcm": EXTRACT / "arm9/dtcm.bin"}
    for d in sorted((CONFIG / "overlays").iterdir()):
        dirs[d.name] = d
        bins[d.name] = EXTRACT / "arm9_overlays" / f"{d.name}.bin"
    mods = {}
    for name, d in dirs.items():
        m = Module(name, d, _sections(d / "delinks.txt"), bins[name].read_bytes())
        m.symbols = read_symbols(d / "symbols.txt")
        m.relocs = read_relocs(d / "relocs.txt")
        m.files = read_files(d / "delinks.txt")
        mods[name] = m
    return mods


def modules_at(mods: dict[str, Module], addr: int) -> list[Module]:
    """Every module whose address range (bss included) holds addr. Overlays
    that share a load address can give more than one."""
    return [m for m in mods.values() if m.start <= addr < m.end]
