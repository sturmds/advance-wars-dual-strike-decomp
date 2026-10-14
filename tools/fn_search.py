"""`fn.py search ADDR`: what owns an address (docs/workflow.md).

For each module that covers the address (overlays share ranges, ARM7's
main-RAM part shares dtcm's): the section and its kind, the symbol that
holds it with the offset, the delinks.txt file, the object as linked, and
for code the original instruction and, in a `complete` file, the C line;
for data the C declaration. The config gives the first three; the rest
needs a build and is left out without one.
"""

import bisect
import re
import sys
from dataclasses import dataclass
from functools import cache
from pathlib import Path

import fn
import modmap
from scan_pointers import declared_size

MAPS = {"arm9": fn.ROOT / "build/arm9.elf.xMAP", "arm7": fn.ROOT / "build/arm7/arm7.elf.xMAP"}
LINKED = {"arm9": fn.ROOT / "build/objects.link.txt", "arm7": fn.ROOT / "build/arm7/objects.link.txt"}
MAP_ENTRY = re.compile(r"^  ([0-9A-F]{8}) ([0-9A-F]{8}) (\.\S+)\s+(\S+)\t\((\S+)\)$")

# DS memory outside the modules, for an address no module covers.
REGIONS = [
    (0x01ff8000, 0x02000000, "ARM9 ITCM, past the itcm module"),
    (0x02000000, 0x02400000, "main RAM outside every module (heap, arena or stack)"),
    (0x027c0000, 0x027c4000, "ARM9 DTCM, past the dtcm module (stack)"),
    (0x027ff000, 0x02800000, "main RAM shared area (BIOS and firmware data)"),
    (0x03000000, 0x03800000, "shared WRAM"),
    (0x03800000, 0x03810000, "ARM7 WRAM"),
    (0x04000000, 0x05000000, "hardware I/O registers"),
    (0x05000000, 0x06000000, "palette RAM"),
    (0x06000000, 0x07000000, "VRAM"),
    (0x07000000, 0x08000000, "OAM"),
    (0x08000000, 0x0a000000, "GBA slot ROM"),
    (0x0a000000, 0x0b000000, "GBA slot RAM"),
    (0xffff0000, 0x100000000, "ARM9 BIOS"),
    (0x02400000, 0x03000000, "a mirror of main RAM"),
]


@dataclass
class Part:
    module: str
    dir: Path
    sections: list[modmap.Section]

    @property
    def start(self) -> int:
        return self.sections[0].start

    @property
    def end(self) -> int:
        return max(s.end for s in self.sections)


@cache
def parts() -> list[Part]:
    """Each module's address range: one for an ARM9 module, one per part
    of ARM7 (each starts at a .text section)."""
    dirs = list(fn.module_dirs())
    if (fn.CONFIG7 / "symbols.txt").exists():
        dirs.append(("arm7", fn.CONFIG7))
    out = []
    for name, d in dirs:
        cur: list[modmap.Section] = []
        for s in modmap._sections(d / "delinks.txt"):
            if s.name == ".text" and cur:
                out.append(Part(name, d, cur))
                cur = []
            cur.append(s)
        if cur:
            out.append(Part(name, d, cur))
    return out


@cache
def symbols(d: Path) -> tuple[list[int], list[modmap.Symbol]]:
    syms = sorted((s for s in modmap.read_symbols(d / "symbols.txt") if not s.kind.startswith("label")),
                  key=lambda s: s.addr)
    return [s.addr for s in syms], syms


def holders(part: Part, sec: modmap.Section, addr: int) -> list[tuple[modmap.Symbol, int, bool]]:
    """(symbol, end, sized) for each symbol whose range holds addr, innermost
    first. The range comes from the size in symbols.txt, or runs to the next
    symbol (sized False) when it has none; it never leaves the section."""
    addrs, syms = symbols(part.dir)
    out = []
    top = bisect.bisect_right(addrs, addr) - 1
    # An object that holds addr starts at most 256 symbols back (aliases inside a table).
    for i in range(top, max(top - 256, -1), -1):
        s = syms[i]
        if s.addr < sec.start:
            break
        size = s.size if s.kind.startswith("function") else declared_size(s.kind)
        if size:
            end = s.addr + size
        else:
            j = bisect.bisect_right(addrs, s.addr)
            end = addrs[j] if j < len(addrs) else sec.end
        end = min(end, sec.end)
        if addr < end:
            out.append((s, end, bool(size)))
    return out


@cache
def files(d: Path) -> list[tuple[int, int, str]]:
    return modmap.read_files(d / "delinks.txt")


def delinks_file(part: Part, addr: int) -> tuple[str, bool] | None:
    """The delinks.txt file with a section range that holds addr, and
    whether it is `complete`."""
    for start, end, path in files(part.dir):
        if start <= addr < end:
            done = {p for p, _, _, c in fn.all_modules()[part.module].files if c}
            return path, path in done
    return None


@cache
def link_map(cpu: str) -> dict[str, list[tuple[int, int, str]]]:
    """Module -> sorted (address, size, object) of every entry in the
    linker's map."""
    out: dict[str, list[tuple[int, int, str]]] = {}
    path = MAPS[cpu]
    if not path.exists():
        return out
    block = None
    for line in path.read_text().splitlines():
        if line.startswith("# ."):
            name = line[3:].strip()
            if cpu == "arm7":
                block = "arm7"
            elif name == "arm9":
                block = "main"
            elif name in ("itcm", "dtcm") or re.fullmatch(r"ov\d{3}", name):
                block = name
            else:
                block = None
        elif block and (m := MAP_ENTRY.match(line)):
            out.setdefault(block, []).append((int(m.group(1), 16), int(m.group(2), 16), m.group(5)))
    for v in out.values():
        v.sort(key=lambda e: e[0])
    return out


@cache
def linked_paths(cpu: str) -> dict[str, list[str]]:
    out: dict[str, list[str]] = {}
    if LINKED[cpu].exists():
        for line in LINKED[cpu].read_text().splitlines():
            p = line.strip().strip('"')
            if p:
                out.setdefault(Path(p).name, []).append(p)
    return out


def linked_object(module: str, addr: int, unit: str | None) -> str | None:
    """The object the linker placed at addr, from its map, as the path in
    objects.link.txt."""
    cpu = "arm7" if module == "arm7" else "arm9"
    entries = link_map(cpu).get(module, [])
    i = bisect.bisect_right(entries, addr, key=lambda e: e[0]) - 1
    if i < 0:
        return None
    obj = entries[i][2]
    paths = linked_paths(cpu).get(obj, [])
    if unit:
        stem = str(Path(unit).with_suffix(".o"))
        paths = [p for p in paths if p.endswith(stem)] or paths
    return paths[0] if paths else obj


def instruction(f: fn.Func, addr: int) -> str | None:
    """The row of the original disassembly at addr, as `fn.py show` prints it."""
    obj = fn.original_object(f)
    if obj is None:
        return None
    off = addr - f.addr
    best = None
    for row in fn.objdump(obj, f.name, f.mode):
        if int(row.split(":", 1)[0], 16) <= off:
            best = row
    return best


def c_line(f: fn.Func, src: Path, addr: int) -> str | None:
    """The C line the instruction at addr comes from, from the line table of
    src compiled with -g. Valid only when src matches (a complete file)."""
    ok, out = fn.line_listing(f, src)
    if not ok:
        return None
    off, here, best = addr - f.addr, None, None
    for line in out.splitlines():
        if m := re.match(r"(\S.*?):(\d+)(?: \(discriminator \d+\))?$", line):
            here = (fn.source_path(m.group(1)), int(m.group(2)))
        elif (m := re.match(r"\s+([0-9a-f]+):\s", line)) and int(m.group(1), 16) <= off:
            best = here
    if not best or not best[0]:
        return None
    path, n = best
    lines = path.read_text(errors="replace").splitlines()
    return f"{fn.rel(path)}:{n}: {lines[n - 1].strip() if n <= len(lines) else ''}"


@cache
def externs() -> dict[str, list[dict]]:
    import dsindex
    return dsindex.externs()


def declarations(name: str, unit: str | None) -> list[str]:
    """The C definition of a data symbol in its own source file, then its
    extern declarations in include/ and src/."""
    out = []
    src = fn.ROOT / unit if unit else None
    if src and src.suffix == ".c" and src.exists():
        pat = re.compile(rf"^(?!extern\b)[^\s(#/][^;=(]*\b{re.escape(name)}\b\s*(\[[^\]]*\])?\s*(=|;)")
        for n, line in enumerate(src.read_text(errors="replace").splitlines(), 1):
            if pat.match(line):
                out.append(f"{line.rstrip(' ={;')}  ({unit}:{n})")
    for d in externs().get(name, [])[:4]:
        dim = "" if d["count"] is None else f"[{d['count']}]"
        out.append(f"extern {d['type']} {name}{dim}  ({d['file']}:{d['line']})")
    return out


def region(addr: int) -> str | None:
    return next((what for lo, hi, what in REGIONS if lo <= addr < hi), None)


def parse_addr(text: str) -> int:
    if not re.fullmatch(r"(0[xX])?[0-9a-fA-F]{1,8}", text):
        raise ValueError(text)
    return int(text, 16)


def describe(part: Part, addr: int, lines: bool) -> list[str]:
    sec = next((s for s in part.sections if s.start <= addr < s.end), None)
    if sec is None:
        return [f"{addr:#010x}  {part.module}  between sections (alignment padding)"]
    out = [f"{addr:#010x}  {part.module}  {sec.name} ({sec.kind})"]
    held = holders(part, sec, addr)
    fi = delinks_file(part, addr)
    unit = fi[0] if fi else None
    if held:
        s, end, sized = held[0]
        what = f"{s.name}+{addr - s.addr:#x}" if addr != s.addr else s.name
        size = f"size {end - s.addr:#x}" + ("" if sized else " to the next symbol")
        kind = re.sub(r"\((arm|thumb).*", r" \1", s.kind).split("(")[0]
        out.append(f"  symbol:  {what}  ({kind} at {s.addr:#010x}, {size})")
        for outer, _, _ in held[1:]:
            out.append(f"  inside:  {outer.name}+{addr - outer.addr:#x}")
    else:
        addrs, syms = symbols(part.dir)
        i = bisect.bisect_right(addrs, addr) - 1
        after = f"after {syms[i].name}" if i >= 0 and syms[i].addr >= sec.start else "before any symbol"
        out.append(f"  symbol:  none ({after} in {sec.name})")
    if fi:
        out.append(f"  file:    {unit}" + (" (complete)" if fi[1] else ""))
    else:
        out.append("  file:    none in delinks.txt (a dsd gap object)")
    obj = linked_object(part.module, addr, unit)
    out.append(f"  linked:  {obj or '(no build yet: make)'}")
    if not held:
        return out
    s = held[0][0]
    if s.kind.startswith("function"):
        f = next(g for g in fn.all_modules()[part.module].funcs if g.addr == s.addr)
        insn = instruction(f, addr)
        out.append(f"  insn:    {insn.strip() if insn else '(no delinked object yet: make)'}")
        if lines and fi and fi[1] and unit.endswith(".c"):
            line = c_line(f, fn.ROOT / unit, addr)
            if line:
                out.append(f"  C line:  {line}")
    else:
        for d in declarations(s.name, unit) or ["not declared in include/ or src/"]:
            out.append(f"  C:       {d}")
    return out


def search(addr: int, module: str | None = None, lines: bool = True) -> list[str]:
    hits = [p for p in parts() if p.start <= addr < p.end and (module is None or p.module == module)]
    if not hits and module:
        others = sorted({p.module for p in parts() if p.start <= addr < p.end})
        if others:
            return [f"{addr:#010x}  not in module {module}; in {', '.join(others)}"]
    if not hits:
        where = region(addr)
        return [f"{addr:#010x}  not in any module" + (f"; {where}" if where else "")]
    out = []
    for p in hits:
        if out:
            out.append("")
        out += describe(p, addr, lines)
    return out


def cmd_search(args) -> int:
    known = {p.module for p in parts()}
    if args.module and args.module not in known:
        sys.exit(f"error: no module {args.module} (known: {', '.join(sorted(known))})")
    blocks = []
    for text in args.addrs:
        try:
            addr = parse_addr(text)
        except ValueError:
            sys.exit(f"error: {text} is not a hex address (0x20D7164, 020d7164, 0x020d7164)")
        blocks.append("\n".join(search(addr, args.module, not args.no_lines)))
    print("\n\n".join(blocks))
    return 0
