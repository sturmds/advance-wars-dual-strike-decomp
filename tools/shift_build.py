#!/usr/bin/env python3
"""Build a ROM with padding inserted in main, and check what moved.

    shift_build.py [--pad 0x20] [--at 0x02004000]

A shifted build tests whether the build can move code (issues 0012, 0023):
every pointer must be a relocation for the linker to fix it up. This script
takes the normal build's linker script and object list, inserts `--pad`
zero bytes into main's .text before the first object that starts at or
after `--at`, and links, configures and builds a ROM in build/shift/. It
never changes build/AWRE.nds or anything `make release` checks. `make shift`
runs it after a normal build.

The header gets the retail secure-area CRC and a new header CRC. The secure
area (main's first 16 KiB) always changes, because it holds pointers to code
after the pad, so that CRC is wrong (see write_header).

It then compares the two ELF files' symbols:
  - itcm and dtcm symbols, and main symbols before the pad, must not move;
  - main symbols after the pad, and every overlay symbol, must move by the
    pad (overlays load right after main's .bss);
and prints any symbol that does neither. Exit 1 on any such symbol.

A shifted ROM that links and passes this check can still crash: a pointer
stored as a plain number keeps its old value (`tools/scan_pointers.py`
lists the suspects). Only running it tells (issue 0006).
"""

import argparse
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

OUT = Path("build/shift")
CONFIG = Path("config/AWRE/arm9/config.yaml")
PATH_KEYS = ("rom_config", "build_path", "delinks_path", "object", "delinks", "symbols", "relocations")
MAP_TEXT_RE = re.compile(r"^\s+([0-9A-F]{8}) [0-9A-F]{8} \.text\s+\.text\s+\((\S+)\)")


def run(cmd: list[str]) -> None:
    print("shift:", " ".join(cmd))
    subprocess.run(cmd, check=True, stdout=subprocess.DEVNULL)


def write_config() -> Path:
    """Copy config.yaml to build/shift/, with paths that point back at the
    real config files and module binaries written under build/shift/build/."""
    src_dir = CONFIG.parent
    out = []
    for line in CONFIG.read_text().splitlines():
        m = re.match(r"^(\s*(?:- )?)(\w+): (\S+)$", line)
        if m and m.group(2) in PATH_KEYS:
            target = os.path.normpath(src_dir / m.group(3))
            if m.group(2) == "object":
                target = target.replace("build/build/", f"{OUT}/build/", 1)
            line = f"{m.group(1)}{m.group(2)}: {os.path.relpath(target, OUT)}"
        out.append(line)
    path = OUT / "config.yaml"
    path.write_text("\n".join(out) + "\n")
    return path


def pad_object(at: int) -> tuple[str, int]:
    """The first object in main whose .text starts at or after `at`."""
    for line in Path("build/arm9.elf.xMAP").read_text().splitlines():
        m = MAP_TEXT_RE.match(line)
        if m and at <= int(m.group(1), 16) < 0x02200000:
            return m.group(2), int(m.group(1), 16)
    raise SystemExit(f"error: no object in main starts at or after {at:#x}")


def write_lcf(obj: str, pad: int) -> Path:
    lcf = Path("build/arm9.lcf").read_text()
    needle = f"        {obj}(.text)\n"
    if lcf.count(needle) != 1:
        raise SystemExit(f"error: {obj}(.text) is not in build/arm9.lcf exactly once")
    lcf = lcf.replace(needle, f"        . = . + {pad:#x};\n" + needle)
    path = OUT / "arm9.lcf"
    path.write_text(lcf)
    return path


def write_header(raw: Path, out: Path) -> None:
    """Copy the retail secure-area CRC and recompute the header CRC, as
    tools/fix_header.py does, but without its check that the secure area is
    unchanged: main's first 16 KiB hold pointers to code after the pad, so
    it always changes here. The secure-area CRC is then wrong. Emulators
    load the ROM without checking it as far as we know; hardware might not."""
    rom = bytearray(raw.read_bytes())
    with open("orig/AWRE.nds", "rb") as f:
        orig = f.read(0x8000)
    rom[fix_header.SECURE_CRC] = orig[fix_header.SECURE_CRC]
    struct.pack_into("<H", rom, fix_header.HEADER_CRC, fix_header.crc16(rom[:fix_header.HEADER_CRC]))
    out.write_bytes(rom)


def elf_symbols(path: Path) -> dict[str, int]:
    """Global and local named symbols with an address, from readelf."""
    text = subprocess.run(["arm-none-eabi-readelf", "-sW", str(path)], check=True,
                          capture_output=True, text=True).stdout
    syms: dict[str, int] = {}
    for line in text.splitlines():
        parts = line.split()
        if len(parts) == 8 and parts[6] != "UND" and parts[3] in ("FUNC", "OBJECT", "NOTYPE"):
            name = parts[7]
            if name.startswith("$") or name.startswith("@"):
                continue
            syms.setdefault(name, int(parts[1], 16))
    return syms


def compare(pad: int, pad_addr: int) -> int:
    base = elf_symbols(Path("build/arm9.elf"))
    moved = elf_symbols(OUT / "arm9.elf")
    counts = {"same": 0, "moved": 0}
    bad = []
    for name, addr in sorted(base.items(), key=lambda kv: kv[1]):
        if name not in moved:
            bad.append(f"  {name} {addr:#010x}: missing from the shifted ELF")
            continue
        # Main and the overlays live below 0x02400000; itcm (0x01ff8000) and
        # dtcm (0x027c0000) do not move.
        expect = addr + pad if pad_addr <= addr < 0x02400000 else addr
        if moved[name] == expect:
            counts["same" if expect == addr else "moved"] += 1
        else:
            bad.append(f"  {name} {addr:#010x} -> {moved[name]:#010x}, expected {expect:#010x}")
    print(f"shift: {counts['moved']} symbols moved by {pad:#x}, {counts['same']} stayed")
    if bad:
        print(f"shift: {len(bad)} symbols did neither:")
        print("\n".join(bad[:50]))
    return 1 if bad else 0


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--pad", type=lambda s: int(s, 0), default=0x20, help="bytes to insert (multiple of 32)")
    ap.add_argument("--at", type=lambda s: int(s, 0), default=0x02004000,
                    help="insert before the first object at or after this address")
    args = ap.parse_args()
    if args.pad % 32:
        raise SystemExit("error: --pad must be a multiple of 32, the largest section alignment in the build")
    os.chdir(ROOT)
    for need in ("build/arm9.lcf", "build/objects.link.txt", "build/arm9.elf.xMAP", "build/arm9.elf"):
        if not Path(need).exists():
            raise SystemExit(f"error: {need} is missing, run `make` first")
    OUT.mkdir(parents=True, exist_ok=True)
    (OUT / "build").mkdir(exist_ok=True)

    obj, addr = pad_object(args.at)
    print(f"shift: {args.pad:#x} bytes before {obj} ({addr:#010x})")
    lcf = write_lcf(obj, args.pad)
    cfg = write_config()
    elf = OUT / "arm9.elf"
    run([sys.executable, "tools/link.py", configure.LD, str(elf), str(lcf), "build/objects.link.txt",
         *configure.LDFLAGS.split()])
    run([configure.DSD, "rom", "config", "-e", str(elf), "-c", str(cfg)])
    raw = OUT / "AWRE.raw.nds"
    run([configure.DSD, "rom", "build", "-c", str(OUT / "build/rom_config.yaml"), "-o", str(raw)])
    write_header(raw, OUT / "AWRE.nds")
    print(f"shift: wrote {OUT / 'AWRE.nds'}")
    return compare(args.pad, addr)


if __name__ == "__main__":
    sys.exit(main())
