#!/usr/bin/env python3
"""Build a ROM with padding inserted in one module, and check what moved.

    shift_build.py [--module main] [--pad 0x20] [--at 0x02004000]

A shifted build tests whether the build can move code (issues 0012, 0023,
0086): every pointer must be a relocation for the linker to fix it up. This
script takes the normal build's linker script and object list, inserts
`--pad` zero bytes into one module, and links, configures and builds a ROM
in build/shift/ (main) or build/shift-MODULE/ (any other module). It never
changes build/AWRE.nds or anything `make release` checks. `make shift` runs
it for main and for ov000 after a normal build.

Where the pad goes:
  main        in .text, before the first object that starts at or after
              `--at`; main's later code and data and every overlay move
  ovNNN       at the start of the overlay's .text; the overlay moves, and so
              do the overlays linked after it (`ORIGIN = AFTER(OVNNN)`)
  itcm, dtcm  at the start of the module; only the module itself moves

The header gets the retail secure-area CRC and a new header CRC. The secure
area (main's first 16 KiB) changes whenever main changes, because it holds
pointers to code after the pad, so that CRC is wrong (see write_header).

Checks, each printed with its count; exit 1 if any fails:
  symbols    every symbol of the normal build either stayed or moved by the
             pad, as the list above says;
  arena      the main arena starts past main and every overlay
             (SDK_MAIN_ARENA_LO, issue 0086);
  stale      a word with no relocation in the shifted build whose value is
             the old address of a symbol that moved (`OS_GetInitArenaLo`
             returning the old end of the overlays, for example). Words in
             config/AWRE/shift_numbers.txt are numbers, read one by one,
             and are only counted.

A shifted ROM that passes can still crash: a pointer to the middle of an
object, or one built at run time, is not seen (`tools/scan_pointers.py`
lists the suspects). Only running it tells (issue 0006).
"""

import argparse
import bisect
import functools
import os
import re
import struct
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import configure  # noqa: E402  (compiler and linker paths, flags)
import fix_header  # noqa: E402

CONFIG = Path("config/AWRE/arm9/config.yaml")
PATH_KEYS = ("rom_config", "build_path", "delinks_path", "object", "delinks", "symbols", "relocations")
MAP_TEXT_RE = re.compile(r"^\s+([0-9A-F]{8}) [0-9A-F]{8} \.text\s+\.text\s+\((\S+)\)")
MEMORY_RE = re.compile(r"^\s+(\w+) : ORIGIN = (?:AFTER\(([\w, ]+)\)|0x[0-9a-f]+)", re.M)

# Words that hold the old address of a moved symbol but are numbers, read
# one by one: `ADDRESS  reason` per line, the address in the normal build.
STALE_NUMBERS = Path("config/AWRE/shift_numbers.txt")


def stale_numbers() -> dict[int, str]:
    out = {}
    for line in STALE_NUMBERS.read_text().splitlines():
        if line.strip() and not line.startswith("#"):
            addr, reason = line.split(None, 1)
            out[int(addr, 16)] = reason
    return out


def run(cmd: list[str]) -> None:
    print("shift:", " ".join(cmd))
    subprocess.run(cmd, check=True, stdout=subprocess.DEVNULL)


def write_config(out: Path) -> Path:
    """Copy config.yaml to OUT, with paths that point back at the real
    config files and module binaries written under OUT/build/."""
    src_dir = CONFIG.parent
    lines = []
    for line in CONFIG.read_text().splitlines():
        m = re.match(r"^(\s*(?:- )?)(\w+): (\S+)$", line)
        if m and m.group(2) in PATH_KEYS:
            target = os.path.normpath(src_dir / m.group(3))
            if m.group(2) == "object":
                target = target.replace("build/build/", f"{out}/build/", 1)
            line = f"{m.group(1)}{m.group(2)}: {os.path.relpath(target, out)}"
        lines.append(line)
    path = out / "config.yaml"
    path.write_text("\n".join(lines) + "\n")
    return path


def pad_object(at: int) -> tuple[str, int]:
    """The object in main whose .text starts first at or after `at`. The
    map lists objects in link order, not by address."""
    found = []
    for line in Path("build/arm9.elf.xMAP").read_text().splitlines():
        m = MAP_TEXT_RE.match(line)
        if m and at <= int(m.group(1), 16) < 0x02200000:
            found.append((int(m.group(1), 16), m.group(2)))
    if not found:
        raise SystemExit(f"error: no object in main starts at or after {at:#x}")
    addr, obj = min(found)
    return obj, addr


def region(module: str) -> str:
    """The MEMORY region and section prefix of a module: ARM9, ITCM, OV000."""
    return "ARM9" if module == "main" else module.upper()


def pad_line(module: str, at: int) -> tuple[str, int]:
    """The line of build/arm9.lcf the pad goes before, and the address from
    which symbols of the module move."""
    if module == "main":
        obj, addr = pad_object(at)
        return f"        {obj}(.text)\n", addr
    first = "DTCM_DATA_START" if module == "dtcm" else f"{region(module)}_TEXT_START"
    return f"        {first} = .;\n", 0


def write_lcf(out: Path, lines: list[str], pad: int) -> Path:
    """build/arm9.lcf with the pad before each object's line, or after each
    `NAME = .;` line."""
    lcf = Path("build/arm9.lcf").read_text()
    gap = f"        . = . + {pad:#x};\n"
    for line in lines:
        if lcf.count(line) != 1:
            raise SystemExit(f"error: {line.strip()!r} is not in build/arm9.lcf exactly once")
        lcf = lcf.replace(line, line + gap if line.endswith("= .;\n") else gap + line)
    path = out / "arm9.lcf"
    path.write_text(lcf)
    return path


def moving_from(module: str, lcf: str) -> set[str]:
    """The MEMORY regions of linker script `lcf` that move when `module`
    grows: the module's own and every region placed AFTER one that moves.
    SPACE is left out."""
    after = {name: [r.strip() for r in deps.split(",")] if deps else []
             for name, deps in MEMORY_RE.findall(lcf)}
    moving = {region(module)}
    changed = True
    while changed:
        changed = False
        for name, deps in after.items():
            if name not in moving and name != "SPACE" and moving & set(deps):
                moving.add(name)
                changed = True
    return moving


@functools.cache
def moving_regions(module: str) -> set[str]:
    return moving_from(module, Path("build/arm9.lcf").read_text())


def readelf(path: Path, flag: str) -> str:
    return subprocess.run(["arm-none-eabi-readelf", flag, "-W", str(path)], check=True,
                          capture_output=True, text=True).stdout


def elf_sections(path: Path) -> dict[int, tuple[str, int]]:
    """Section index -> (name, address)."""
    out = {}
    for m in re.finditer(r"^\s*\[\s*(\d+)\] (\S+)\s+\S+\s+([0-9a-f]{8})", readelf(path, "-S"), re.M):
        out[int(m.group(1))] = (m.group(2), int(m.group(3), 16))
    return out


def elf_symbols(path: Path) -> dict[str, tuple[int, str]]:
    """Named symbols with an address: name -> (address, ELF section name or
    ABS). The first definition of a name wins."""
    sections = elf_sections(path)
    syms: dict[str, tuple[int, str]] = {}
    for line in readelf(path, "-s").splitlines():
        parts = line.split()
        if len(parts) == 8 and parts[6] != "UND" and parts[3] in ("FUNC", "OBJECT", "NOTYPE"):
            name = parts[7]
            if name.startswith("$") or name.startswith("@"):
                continue
            where = sections[int(parts[6])][0] if parts[6].isdigit() else parts[6]
            syms.setdefault(name, (int(parts[1], 16), where))
    return syms


def module_bins(elf: Path, bins: Path) -> dict[str, tuple[int, bytes]]:
    """Region -> (load address, bytes) of each ARM9 module a link wrote."""
    files = {"ARM9": "arm9.bin", "ITCM": "itcm.bin", "DTCM": "dtcm.bin"}
    files |= {f"OV{n:03d}": f"arm9_ov{n:03d}.bin" for n in range(7)}
    sections = {name: addr for name, addr in elf_sections(elf).values()}
    return {name: (sections[name], (bins / f).read_bytes()) for name, f in files.items()}


# The address range each region's symbols can lie in. Symbols that the
# linker script assigns outside any section (`__DTCM_LO`, `OVERLAY_1_ID`)
# get the index of some region's section, so the range decides too.
REGION_RANGE = {"ITCM": (0x01FF8000, 0x02000000), "DTCM": (0x027C0000, 0x027C4000)}
MAIN_RAM = (0x02000000, 0x02400000)


def shift_of(addr: int, where: str, pads: list[tuple[str, int]], pad: int) -> int:
    """How far a symbol of the normal build moves: `pad` for each padded
    module whose growth moves its region, if it lies past that pad."""
    lo, hi = REGION_RANGE.get(where, MAIN_RAM)
    if not lo <= addr < hi:
        return 0
    total = 0
    for module, pad_addr in pads:
        if where in moving_regions(module) and (where != region(module) or addr >= pad_addr):
            total += pad
    return total


def check_symbols(base: dict, moved: dict, pads: list[tuple[str, int]], pad: int) -> int:
    """Every symbol stayed or moved by the pads, as the module list says."""
    layout = set(configure.ARM9_LAYOUT_SYMBOLS)
    # The label a module's pad goes right after moves only with the others.
    labels = {"DTCM_DATA_START" if m == "dtcm" else f"{region(m)}_TEXT_START": m for m, _ in pads}
    counts = {"same": 0, "moved": 0}
    bad = []
    for name, (addr, where) in sorted(base.items(), key=lambda kv: kv[1][0]):
        if name in layout:
            continue  # checked by check_arena
        if name not in moved:
            bad.append(f"  {name} {addr:#010x}: missing from the shifted ELF")
            continue
        own = [p for p in pads if p[0] != labels.get(name)]
        expect = addr + shift_of(addr, where, own, pad)
        if moved[name][0] == expect:
            counts["same" if expect == addr else "moved"] += 1
        else:
            bad.append(f"  {name} {addr:#010x} -> {moved[name][0]:#010x}, expected {expect:#010x}")
    print(f"shift: symbols: {counts['moved']} moved, {counts['same']} stayed")
    if bad:
        print(f"shift: symbols: {len(bad)} did neither:")
        print("\n".join(bad[:50]))
    return 1 if bad else 0


def check_arena(moved: dict) -> int:
    """The main arena starts at or past the end of main and every overlay."""
    arena = moved.get("SDK_MAIN_ARENA_LO", (None,))[0]
    ends = {n: a for n, (a, _) in moved.items() if re.fullmatch(r"(ARM9|OV\d{3})_BSS_END", n)}
    last = max(ends, key=ends.get)
    if arena is None:
        print("shift: arena: SDK_MAIN_ARENA_LO is not defined")
        return 1
    if arena < ends[last] or arena % 32:
        print(f"shift: arena: starts at {arena:#010x}, inside the modules ({last} = {ends[last]:#010x})")
        return 1
    print(f"shift: arena: starts at {arena:#010x}, past {last} ({ends[last]:#010x})")
    return 0


# Regions that load at the same addresses: a word in one never points into
# another of the same group, so an old address there is not a stale pointer.
OVERLAPPING = [{"OV000", "OV004"}, {"OV001", "OV002", "OV003", "OV005", "OV006"}]


def visible(reg: str, where: str) -> bool:
    """True when code or data in region `reg` can point into `where`."""
    return reg == where or not any(reg in g and where in g for g in OVERLAPPING)


def check_stale(base: dict, moved: dict, out: Path, pads: list[tuple[str, int]], pad: int) -> int:
    """Words that hold the old address of a symbol that moved, and that the
    link left unchanged: a relocation would have moved them with it."""
    old: dict[int, list[tuple[str, str]]] = {}
    for name, (addr, where) in base.items():
        if name in moved and moved[name][0] != addr and addr >= 0x01000000:
            old.setdefault(addr, []).append((name, where))
    before = module_bins(Path("build/arm9.elf"), Path("build/build"))
    after = module_bins(out / "arm9.elf", out / "build")
    numbers = stale_numbers()
    hits, known = [], 0
    for reg, (start, data) in before.items():
        new_data = after[reg][1]
        holders = sorted((a, n) for n, (a, w) in base.items() if w == reg)
        starts = [a for a, _ in holders]
        for off in range(0, len(data) - 3, 4):
            value = int.from_bytes(data[off:off + 4], "little")
            targets = [n for n, w in old.get(value, []) + old.get(value - 1, []) if visible(reg, w)]
            if not targets:  # value - 1: a Thumb function's address is odd
                continue
            addr = start + off
            new_off = off + sum(pad for m, at in pads if reg == region(m) and addr >= at)
            if new_data[new_off:new_off + 4] != data[off:off + 4]:
                continue  # relocated: the link moved it
            if addr in numbers:
                known += 1
                continue
            at, holder = holders[bisect.bisect_right(starts, addr) - 1]
            hits.append(f"  {reg} {addr:#010x} ({holder}+{addr - at:#x}) = {value:#010x},"
                        f" the old address of {targets[0]}")
    print(f"shift: stale: {len(hits)} words kept the old address of a moved symbol"
          f" ({known} known, left out)")
    if hits:
        print("\n".join(hits[:50]))
    return 1 if hits else 0


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--module", action="append",
                    help="main, itcm, dtcm or ovNNN; repeat to grow several (default main)")
    ap.add_argument("--pad", type=lambda s: int(s, 0), default=0x20,
                    help="bytes to insert in each module (multiple of 32)")
    ap.add_argument("--at", type=lambda s: int(s, 0), default=0x02004000,
                    help="main only: insert before the first object at or after this address")
    args = ap.parse_args()
    modules = sorted(set(args.module or ["main"]))
    if args.pad % 32:
        raise SystemExit("error: --pad must be a multiple of 32, the largest section alignment in the build")
    for module in modules:
        if not re.fullmatch(r"main|itcm|dtcm|ov00[0-6]", module):
            raise SystemExit(f"error: unknown module {module}")
    os.chdir(ROOT)
    for need in ("build/arm9.lcf", "build/objects.link.txt", "build/arm9.elf.xMAP", "build/arm9.elf"):
        if not Path(need).exists():
            raise SystemExit(f"error: {need} is missing, run `make` first")
    out = Path("build/shift" if modules == ["main"] else f"build/shift-{'-'.join(modules)}")
    (out / "build").mkdir(parents=True, exist_ok=True)

    lines, pads = [], []
    for module in modules:
        line, addr = pad_line(module, args.at)
        print(f"shift: {args.pad:#x} bytes in {module}, at {line.strip()}")
        lines.append(line)
        pads.append((module, addr))
    lcf = write_lcf(out, lines, args.pad)
    cfg = write_config(out)
    elf = out / "arm9.elf"
    run([sys.executable, "tools/link.py", configure.LD, str(elf), str(lcf), "build/objects.link.txt",
         *configure.LDFLAGS.split()])
    run([configure.DSD, "rom", "config", "-e", str(elf), "-c", str(cfg)])
    raw = out / "AWRE.raw.nds"
    run([configure.DSD, "rom", "build", "-c", str(out / "build/rom_config.yaml"), "-o", str(raw)])
    write_header(raw, out / "AWRE.nds")
    print(f"shift: wrote {out / 'AWRE.nds'}")
    base, moved = elf_symbols(Path("build/arm9.elf")), elf_symbols(elf)
    failed = check_symbols(base, moved, pads, args.pad)
    failed |= check_arena(moved)
    failed |= check_stale(base, moved, out, pads, args.pad)
    return failed


def write_header(raw: Path, out: Path) -> None:
    """Copy the retail secure-area CRC and recompute the header CRC, as
    tools/fix_header.py does, but without its check that the secure area is
    unchanged: main's first 16 KiB hold pointers to code after the pad, so
    it changes whenever main does. The secure-area CRC is then wrong.
    Emulators load the ROM without checking it as far as we know; hardware
    might not."""
    rom = bytearray(raw.read_bytes())
    with open("orig/AWRE.nds", "rb") as f:
        orig = f.read(0x8000)
    rom[fix_header.SECURE_CRC] = orig[fix_header.SECURE_CRC]
    struct.pack_into("<H", rom, fix_header.HEADER_CRC, fix_header.crc16(rom[:fix_header.HEADER_CRC]))
    out.write_bytes(rom)


if __name__ == "__main__":
    sys.exit(main())
