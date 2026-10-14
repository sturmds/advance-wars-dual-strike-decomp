#!/usr/bin/env python3
"""Find words that look like addresses but have no relocation (issue 0012).

A shifted build (code or data inserted, so later addresses move) only works
if every pointer is a relocation: the linker rewrites relocations, never a
plain number. dsd found most pointers when it made relocs.txt; this lists
the 4-byte words it may have missed.

It reads every ARM9 module's .rodata and .data, and the words in .text that
are not instructions: the literal pool at the end of each function (the
constants a function loads with `ldr rX, =...`; pool offsets from
build/fnindex.json) and data between functions. A word is relocated when
relocs.txt or a linked object (build/objects.link.txt, placed by
build/arm9.elf.xMAP) has a 32-bit relocation on it; the second covers
pointers written in C that dsd never saw. Every other word is a candidate
when its value falls inside a module's address range (main, itcm, dtcm or an
overlay, .bss included), within 0x20 bytes after a boundary (below), or,
for a literal pool, in ITCM, main RAM or DTCM. Values in FIXED are left out:
hardware and SDK addresses that do not move. Each candidate gets a level:

  layout  the value is a module or section start or end, or the end of the
          overlays, or up to 0x20 bytes after one: the bounds that crt0 and
          the SDK arena code load as numbers. Tested first: a module's end is
          not inside the module, so the in-range tests below miss it.
  high    the value is the start of a known symbol (or a Thumb function's
          address + 1), and a relocated word sits 4 or 8 bytes away (it
          looks like one more entry of a pointer table)
  medium  the value is the start of a known symbol, or it is 4-aligned with
          a relocated neighbour, or a literal pool holds a RAM address
          outside every module
  low     anything else in range: often two 16-bit numbers whose upper half
          happens to be 0x01ff-0x0235, or 0x027c-0x027d

Usage:
  scan_pointers.py                      counts by module, section and level
  scan_pointers.py --list high          one line per candidate of that level
  scan_pointers.py --list all --module ov000 --tsv FILE
  scan_pointers.py --unused-symbols     data symbols nothing refers to

The TSV names the symbol or function that holds each word and its source
file. Needs extract/ and build/fnindex.json (`make fnindex`); the linked
objects and --unused-symbols need a build (`make`). Reads only.

--unused-symbols lists, for ARM9 and ARM7, every symbol in symbols.txt that
is not a function, that no relocs.txt entry targets, that no linked object
has a relocation to, and whose name appears nowhere in src/, include/ or
build/gen/. Each gets a group:

  alias   defined by the linker script inside another object (aliases.txt,
          or `tools/maps.py lcf` for Ov000_gMapHeaders), or a label inside
          a gap object: a leftover of a deleted false relocation, most often
  object  the start of an object that a C file or a generated file defines:
          deleting it changes the layout, so it stays until its object is
          merged with a neighbour by hand
"""

import argparse
import bisect
import json
import re
import sys
from collections import Counter
from pathlib import Path

import fnindex
import modmap

LEVELS = ("layout", "high", "medium", "low")

# Values a literal pool may hold that are not pointers into a module: fixed
# addresses of the hardware or the SDK, which no shift moves.
FIXED = {
    0x02000000: "start of main RAM (OSi_ExceptionHandler's range check)",
    0x0200002b: "protection region word: base 0x02000000, 4 MB, enabled (func_0210e4bc)",
    0x0200002d: "protection region word: base 0x02000000, 8 MB, enabled (init_cp15, Boot_InitHardware)",
    0x023c0000: "top of the main arena (OS_GetInitArenaHi, a mov immediate)",
    0x023fe800: "download play host slot in the system area (DLPLAY_HOST_DESC_SLOT, MbFile_*)",
    0x023fe940: "boot copy in the system area (CARDi_InitCommon)",
    0x02400000: "end of main RAM (data_unpack.c, nvram.h, mic.h)",
    0x027c0023: "protection region word: base 0x027c0000, enabled (init_cp15)",
    0x027c3000: "system area at the top of DTCM (SYSTEM_DTCM_BASE, OS_InitException)",
}

# Address ranges a literal pool word is checked against, beyond the modules.
RAM = ((0x01ff8000, 0x02000000, "itcm"), (0x02000000, 0x02400000, "main RAM"), (0x027c0000, 0x027c4000, "dtcm"))

LAYOUT_SPAN = 0x20  # how far after a boundary a value counts as layout

XMAP_LINE = re.compile(r"^  ([0-9A-F]{8}) ([0-9A-F]{8}) (\.\S+)\s+(\S+)\t\((\S+)\)")
XMAP_LCF = re.compile(r"^#>([0-9A-F]{8})\s+(\S+) \(linker command file\)")
R_ARM_ABS32 = 2


def instruction_ranges(index_path) -> dict[str, list[tuple[int, int]]]:
    """Per module, the sorted [start, end) ranges of function instructions,
    literal pools left out."""
    ranges: dict[str, list[tuple[int, int]]] = {}
    for f in json.load(open(index_path))["functions"]:
        addr, pool = int(f["addr"]), int(f["pool"])
        ranges.setdefault(f["module"], []).append((addr, addr + pool))
    return {m: sorted(r) for m, r in ranges.items()}


def code_words(start: int, end: int, insns: list[tuple[int, int]]):
    """Aligned word addresses in [start, end) outside every instruction
    range, each with ".text pool" (right after a function) or ".text other"
    (between functions: crt0's module parameters, for example)."""
    i = 0
    after_function = False
    addr = (start + 3) & ~3
    while addr + 4 <= end:
        while i < len(insns) and insns[i][1] <= addr:
            i += 1
            after_function = True
        if i < len(insns) and insns[i][0] < addr + 4:
            addr = (insns[i][1] + 3) & ~3
            after_function = True
            continue
        yield addr, ".text pool" if after_function else ".text other"
        addr += 4


# ---------------------------------------------------------------- linked objects

def read_xmap(path: Path) -> tuple[dict[tuple[str, str], list[int]], dict[str, int]]:
    """From a linker map: the addresses of each (symbol, object file name)
    that an object defines, and the symbols the linker script defines."""
    placed: dict[tuple[str, str], list[int]] = {}
    lcf: dict[str, int] = {}
    for line in open(path, errors="replace"):
        m = XMAP_LINE.match(line)
        if m:
            placed.setdefault((m.group(4), m.group(5)), []).append(int(m.group(1), 16))
            continue
        m = XMAP_LCF.match(line)
        if m:
            lcf[m.group(2)] = int(m.group(1), 16)
    return placed, lcf


def object_source(obj: str) -> str:
    """build/align2/src/x.o -> src/x.c; build/delinks/_dsd_gap@main_0.o -> the
    gap's module (main)."""
    parts = Path(obj).parts
    if "delinks" in parts:
        return Path(obj).stem.split("@")[1].rsplit("_", 1)[0]
    return str(Path(*parts[parts.index("src"):]).with_suffix(".c"))


def linked_objects(objects_txt: Path, xmap: Path):
    """Each linked object: (path, symbols, relocations by section, the
    linked address of each section the map places)."""
    placed, _ = read_xmap(xmap)
    for line in open(objects_txt):
        path = modmap.ROOT / line.strip().strip('"')
        secs, syms, relas = fnindex.read_elf(path.read_bytes())
        base: dict[int, int] = {}
        for s in syms:
            if 0 < s.shndx < 0xff00 and s.shndx not in base and s.name and not s.name.startswith("$"):
                addrs = placed.get((s.name, path.name))
                if addrs and len(addrs) == 1:
                    base[s.shndx] = addrs[0] - s.value
        yield path, syms, relas, base


def linked_relocations(mods: dict[str, modmap.Module]) -> dict[str, set[int]] | None:
    """Per ARM9 module, the addresses of the words that a linked object
    relocates with R_ARM_ABS32. None without a build."""
    objects_txt, xmap = modmap.ROOT / "build/objects.link.txt", modmap.ROOT / "build/arm9.elf.xMAP"
    if not (objects_txt.exists() and xmap.exists()):
        return None
    module_of = {}
    for m in mods.values():
        module_of[m.name] = m.name
        for _, _, path in m.files:
            module_of[path] = m.name
    out: dict[str, set[int]] = {m: set() for m in mods}
    for path, _, relas, base in linked_objects(objects_txt, xmap):
        mod = module_of.get(object_source(str(path.relative_to(modmap.ROOT))))
        if mod is None:
            continue
        for sec, entries in relas.items():
            if sec in base:
                out[mod].update(base[sec] + off for off, rtype, _, _ in entries if rtype == R_ARM_ABS32)
    return out


# ---------------------------------------------------------------- word levels

def boundaries(mods: dict[str, modmap.Module]) -> dict[int, str]:
    """Module and section starts and ends, and the end of the overlays."""
    out: dict[int, str] = {}
    for m in mods.values():
        for s in m.sections:
            out.setdefault(s.start, f"{m.name} {s.name} start")
            out.setdefault(s.end, f"{m.name} {s.name} end")
        out.setdefault(m.end, f"{m.name} end")
    overlays = [m.end for m in mods.values() if m.name.startswith("ov")]
    if overlays:
        out[max(overlays)] = "end of the overlays"
    return out


class Levels:
    """Gives each unrelocated word its level, or None when it is no
    candidate."""

    def __init__(self, mods: dict[str, modmap.Module]):
        self.mods = mods
        self.starts: dict[str, set[int]] = {}
        for m in mods.values():
            s = self.starts.setdefault(m.name, set())
            for sym in m.symbols:
                s.add(sym.addr)
                if sym.kind.startswith("function(thumb"):
                    s.add(sym.addr | 1)
        self.bounds = boundaries(mods)
        self.bound_keys = sorted(self.bounds)

    def layout(self, value: int) -> str | None:
        """The boundary at or up to LAYOUT_SPAN bytes below value."""
        i = bisect.bisect_right(self.bound_keys, value) - 1
        if i >= 0 and value - self.bound_keys[i] <= LAYOUT_SPAN:
            b = self.bound_keys[i]
            return self.bounds[b] + (f" + {value - b:#x}" if value != b else "")
        return None

    def level(self, value: int, pool: bool, neighbour: bool) -> tuple[str, str] | None:
        """(level, what the value may point at)."""
        if value in FIXED:
            return None
        bound = self.layout(value)
        if bound:
            return "layout", bound
        targets = modmap.modules_at(self.mods, value & ~1)
        symbol = [t.name for t in targets if value in self.starts[t.name]]
        if symbol and neighbour:
            return "high", "/".join(t.name for t in targets)
        if targets:
            level = "medium" if symbol or (value % 4 == 0 and neighbour) else "low"
            return level, "/".join(t.name for t in targets)
        if pool:
            ram = next((r[2] for r in RAM if r[0] <= value < r[1]), None)
            if ram:
                return "medium", ram + ", no module"
        return None


# ---------------------------------------------------------------- holders

class Holders:
    """The symbol or function that holds an address, and its source file."""

    def __init__(self, mods: dict[str, modmap.Module], index_path: Path):
        self.mods = mods
        self.syms = {m.name: sorted((s.addr, s.name) for s in m.symbols if not s.kind.startswith("function"))
                     for m in mods.values()}
        self.funcs: dict[str, list[tuple[int, int, str, str]]] = {}
        for f in json.load(open(index_path))["functions"]:
            self.funcs.setdefault(f["module"], []).append((int(f["addr"]), int(f["size"]), f["name"],
                                                           f.get("file") or ""))
        for v in self.funcs.values():
            v.sort()

    def file(self, mod: str, addr: int) -> str:
        files = self.mods[mod].files
        i = bisect.bisect_right(files, (addr, 0xffffffff, "")) - 1
        while i >= 0 and files[i][0] <= addr:
            if addr < files[i][1]:
                return files[i][2]
            i -= 1
        return ""

    def holder(self, mod: str, addr: int, pool: bool) -> tuple[str, str]:
        if pool:
            funcs = self.funcs.get(mod, [])
            i = bisect.bisect_right(funcs, (addr, 0xffffffff)) - 1
            if i >= 0:
                start, _, name, path = funcs[i]
                return f"{name}+{addr - start:#x}", path or self.file(mod, addr)
            return "", self.file(mod, addr)
        syms = self.syms[mod]
        i = bisect.bisect_right(syms, (addr, "\x7f")) - 1
        if i < 0:
            return "", self.file(mod, addr)
        start, name = syms[i]
        return name + (f"+{addr - start:#x}" if addr != start else ""), self.file(mod, addr)


# ---------------------------------------------------------------- unused symbols

def referenced_names(roots: list[Path]) -> set[str]:
    """Every identifier in the C sources and headers under roots."""
    words: set[str] = set()
    for root in roots:
        if not root.exists():
            continue
        for p in root.rglob("*"):
            if p.suffix in (".c", ".h", ".inc", ".s") and p.is_file():
                words.update(re.findall(r"[A-Za-z_]\w*", p.read_text(errors="replace")))
    return words


def unused_symbols(symbols: list[tuple[str, modmap.Symbol]], targets: set[tuple[str, int]], relocated: set[str],
                   named: set[str], defined: set[str], lcf: set[str]) -> list[tuple[str, modmap.Symbol, str]]:
    """(module, symbol, group) for each symbol that is not a function, not a
    relocs.txt target (module, address) and not the symbol of a relocation in
    a linked object (`relocated`). One that an object defines (`defined`) is
    an "object"; any other is an "alias" when the sources do not name it
    (`named`) and it is in the linker script (`lcf`) or inside the sized
    symbol before it."""
    out = []
    by_module: dict[str, list[modmap.Symbol]] = {}
    for mod, sym in symbols:
        by_module.setdefault(mod, []).append(sym)
    for mod, syms in by_module.items():
        covered_to = 0  # end of the furthest sized symbol so far
        for sym in sorted(syms, key=lambda s: s.addr):
            inside = sym.addr < covered_to
            covered_to = max(covered_to, sym.addr + declared_size(sym.kind))
            if sym.kind.startswith(("function", "label")):
                continue
            if (mod, sym.addr) in targets or sym.name in relocated:
                continue
            if sym.name in defined:
                out.append((mod, sym, "object"))
            elif sym.name not in named and (sym.name in lcf or inside):
                out.append((mod, sym, "alias"))
    return out


def declared_size(kind: str) -> int:
    """The size in `data(byte[N])` or `bss(size=N)`, else 0."""
    m = re.search(r"\[(0x[0-9a-f]+|\d+)\]\)$|size=(0x[0-9a-f]+)", kind)
    if not m:
        return 0
    return int(m.group(1) or m.group(2), 0)


def cmd_unused(mods: dict[str, modmap.Module], out) -> int:
    builds = [(modmap.ROOT / "build/objects.link.txt", modmap.ROOT / "build/arm9.elf.xMAP"),
              (modmap.ROOT / "build/arm7/objects.link.txt", modmap.ROOT / "build/arm7/arm7.elf.xMAP")]
    if not all(o.exists() and x.exists() for o, x in builds):
        raise SystemExit("error: --unused-symbols needs a build, run `make` first")
    arm7_dir = modmap.ROOT / "config/AWRE/arm7"
    symbols = [(m.name, s) for m in mods.values() for s in m.symbols]
    symbols += [("arm7", s) for s in modmap.read_symbols(arm7_dir / "symbols.txt")]
    targets = set()
    for m in mods.values():
        for r in m.relocs.values():
            targets.update((c, r.to) for c in r.candidates())
    for r in modmap.read_relocs(arm7_dir / "relocs.txt").values():
        targets.update((c, r.to) for c in r.candidates())
    named = referenced_names([modmap.ROOT / "src", modmap.ROOT / "include", modmap.ROOT / "build/gen"])
    relocated: set[str] = set()
    defined: set[str] = set()
    lcf: set[str] = set()
    for objects_txt, xmap in builds:
        placed, by_lcf = read_xmap(xmap)
        lcf.update(by_lcf)
        defined.update(name for name, _ in placed)
        for _, syms, relas, _ in linked_objects(objects_txt, xmap):
            for entries in relas.values():
                relocated.update(syms[si].name for _, _, si, _ in entries)
    rows = unused_symbols(symbols, targets, relocated, named, defined, lcf)
    print("group\tmodule\tsymbol\taddress\tkind", file=out)
    for mod, sym, group in sorted(rows, key=lambda r: (r[2], r[0], r[1].addr)):
        print(f"{group}\t{mod}\t{sym.name}\t{sym.addr:#010x}\t{sym.kind}", file=out)
    counts = Counter(r[2] for r in rows)
    print(f"unused symbols: {counts['alias']} alias, {counts['object']} object", file=sys.stderr)
    return 0


# ---------------------------------------------------------------- main

def candidates(mods: dict[str, modmap.Module], index: Path, module: str | None = None):
    """Every unrelocated word that looks like an address, as
    (level, module, section, addr, value, target, symbol, neighbour, holder, path),
    and the number of words only a linked object relocates (None without a
    build). Used by
    tools/shift_gaps.py too."""
    insns = instruction_ranges(index)
    linked = linked_relocations(mods)
    if linked is None:
        print("note: no build, so only relocs.txt counts as relocated (run `make`)", file=sys.stderr)
    levels = Levels(mods)
    holders = Holders(mods, index)

    rows = []
    only_linked = 0  # words a linked object relocates and relocs.txt does not
    for m in mods.values():
        if module and m.name != module:
            continue
        relocated = set(m.relocs) | (linked or {}).get(m.name, set())
        only_linked += len(relocated) - len(m.relocs)
        words = []
        for sec in m.sections:
            end = min(sec.end, m.init_end)
            if sec.kind in ("rodata", "data"):
                words += [(a, sec.name) for a in range((sec.start + 3) & ~3, end - 3, 4)]
            elif sec.kind == "code":
                words += list(code_words(sec.start, end, insns.get(m.name, [])))
        for addr, section in words:
            if addr in relocated:
                continue
            value = m.word(addr)
            neighbour = any(addr + d in relocated for d in (-8, -4, 4, 8))
            pool = section.startswith(".text")
            got = levels.level(value, pool, neighbour)
            if got is None:
                continue
            level, target = got
            targets = modmap.modules_at(mods, value & ~1)
            symbol = next((t.name for t in targets if value in levels.starts[t.name]), None)
            name = ""
            if symbol:
                name = next((s.name + ("+1" if s.addr != value else "") for s in mods[symbol].symbols
                             if s.addr in (value, value & ~1)), "")
            holder, path = holders.holder(m.name, addr, pool)
            rows.append((level, m.name, section, addr, value, target, name, neighbour, holder, path))
    return rows, (only_linked if linked is not None else None)


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--list", choices=LEVELS + ("all",), help="print candidates of this level")
    ap.add_argument("--module", help="only this module (main, itcm, dtcm, ov000...)")
    ap.add_argument("--tsv", help="write candidates to this file instead of stdout")
    ap.add_argument("--unused-symbols", action="store_true", help="list data symbols nothing refers to")
    args = ap.parse_args()

    index = modmap.ROOT / "build/fnindex.json"
    mods = modmap.load()
    if args.unused_symbols:
        return cmd_unused(mods, open(args.tsv, "w") if args.tsv else sys.stdout)
    if not index.exists():
        raise SystemExit("error: build/fnindex.json is missing, run `make fnindex` first")
    rows, only_linked = candidates(mods, index, args.module)

    counts = Counter((r[1], r[2], r[0]) for r in rows)
    print(f"{'module':7} {'section':12} " + " ".join(f"{lv:>7}" for lv in LEVELS), file=sys.stderr)
    totals = Counter()
    for mod, section in sorted({(k[0], k[1]) for k in counts}):
        n = [counts[(mod, section, lv)] for lv in LEVELS]
        totals.update(dict(zip(LEVELS, n)))
        print(f"{mod:7} {section:12} " + " ".join(f"{x:7}" for x in n), file=sys.stderr)
    print(f"{'total':20} " + " ".join(f"{totals[lv]:7}" for lv in LEVELS), file=sys.stderr)
    if only_linked is not None:
        print(f"{only_linked} words relocated by the linked objects only", file=sys.stderr)

    if args.list:
        out = open(args.tsv, "w") if args.tsv else sys.stdout
        print("level\tmodule\tsection\taddress\tvalue\ttarget\tsymbol\trelocated neighbour\tholder\tfile", file=out)
        for level, mod, section, addr, value, target, name, nb, holder, path in rows:
            if args.list in (level, "all"):
                print(f"{level}\t{mod}\t{section}\t{addr:#010x}\t{value:#010x}\t{target}\t{name}\t"
                      f"{'yes' if nb else 'no'}\t{holder}\t{path}", file=out)
    return 0


if __name__ == "__main__":
    sys.exit(main())
