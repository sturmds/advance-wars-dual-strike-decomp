#!/usr/bin/env python3
"""Let objects start at an address that is 2 mod 4.

usage: lcf_align2.py lcf LCF DELINKS [BLOCK...]
       lcf_align2.py objects LCF OBJECTS OUT_OBJECTS OUT_DIR

Thumb code can start at any even address, and some Thumb functions (the
BIOS call stubs in ARM9 main at 0x02000000-0x020007ff and in ARM7 WRAM at
0x03803b8c-0x03803bb8) start at 2 mod 4. Two things in the link push such
an object to the next multiple of 4: `ALIGNALL(4)`, which dsd lcf writes at
the top of each section block, and the .text section alignment of 4 in the
object itself (mwccarm and dsd both write 4). mwldarm aligns the start of
each object to the larger of the two; it never pads the end. See
docs/tool-issues.md.

`lcf` runs after dsd lcf. It finds, in DELINKS, every .text range that
starts or ends at 2 mod 4. The object that starts there (the file itself,
or the gap or file after it) goes between `ALIGNALL(2);` and
`ALIGNALL(4);` in its text list. Every other object keeps ALIGNALL(4).
BLOCK names the text lists to look in, by the prefix of their
`<BLOCK>_TEXT_START` symbol: ARM9 (the default) for main, AUTOLOAD_2,
AUTOLOAD_3 and AUTOLOAD_4 for ARM7's three parts (tools/arm7.py).

`objects` runs before the link. It copies each object that sits between
those two lines into OUT_DIR, with .text alignment 2, and writes
OUT_OBJECTS: the list from OBJECTS with those paths replaced by the
copies. The copies keep their file names, which the LCF uses.
"""

import re
import struct
import sys
from pathlib import Path

ON = "ALIGNALL(2);"
OFF = "ALIGNALL(4);"
OBJECT_LINE = re.compile(r"^(\s*)(\S+\.o)\(\.text\)$")
TEXT_RANGE = re.compile(r"^\s+\.text\s+start:(0x[0-9a-fA-F]+)\s+end:(0x[0-9a-fA-F]+)")


def odd_ranges(delinks: str) -> tuple[set[str], set[str]]:
    """(objects whose .text starts at 2 mod 4, objects whose .text ends
    at 2 mod 4), by object file name, from one delinks.txt."""
    starts, ends = set(), set()
    name = None
    for line in delinks.splitlines():
        if line and not line[0].isspace() and line.endswith(":"):
            name = Path(line[:-1]).stem + ".o"
            continue
        m = TEXT_RANGE.match(line)
        if m and name is not None:
            if int(m.group(1), 16) % 4 == 2:
                starts.add(name)
            if int(m.group(2), 16) % 4 == 2:
                ends.add(name)
    return starts, ends


def text_list(lines: list[str], block: str) -> list[tuple[int, re.Match]]:
    """(line number, match) of each object line between <block>_TEXT_START
    and <block>_TEXT_END. Raises ValueError when the lcf has no such list."""
    start, end = f"{block}_TEXT_START = .;", f"{block}_TEXT_END = .;"
    stripped = [l.strip() for l in lines]
    if start not in stripped or end not in stripped:
        raise ValueError(f"no {block} text list")
    first = stripped.index(start) + 1
    last = stripped.index(end)
    objects = [(i, OBJECT_LINE.match(lines[i])) for i in range(first, last)]
    return [(i, m) for i, m in objects if m]


def mark(lcf: str, delinks: str, blocks: tuple[str, ...] = ("ARM9",)) -> str:
    """lcf with ALIGNALL(2) around each .text object in the named text lists
    that starts at 2 mod 4. Raises ValueError when a named object is in
    none of them."""
    if ON in lcf:
        return lcf
    starts, ends = odd_ranges(delinks)
    if not starts and not ends:
        return lcf
    lines = lcf.split("\n")
    lists = [text_list(lines, b) for b in blocks]
    names = {m.group(2) for objects in lists for _, m in objects}
    missing = sorted((starts | ends) - names)
    if missing:
        raise ValueError("not in the text lists " + ", ".join(blocks) + ": " + ", ".join(missing))
    inserts = []
    for objects in lists:
        names = [m.group(2) for _, m in objects]
        marked = set()
        for k, name in enumerate(names):
            if name in starts:
                marked.add(k)
            if name in ends and k + 1 < len(names):
                marked.add(k + 1)
        for k in marked:
            i, m = objects[k]
            indent = m.group(1)
            if k + 1 not in marked:
                inserts.append((i + 1, indent + OFF))
            if k - 1 not in marked:
                inserts.append((i, indent + ON))
    out = list(lines)
    # Insert from the bottom so earlier line numbers stay valid. No two
    # inserts share a line number.
    for i, text in sorted(inserts, key=lambda t: t[0], reverse=True):
        out.insert(i, text)
    return "\n".join(out)


def marked_names(lcf: str) -> set[str]:
    """File names of the objects between ALIGNALL(2) and ALIGNALL(4)."""
    names, inside = set(), False
    for line in lcf.splitlines():
        s = line.strip()
        if s == ON:
            inside = True
        elif s == OFF:
            inside = False
        elif inside:
            m = OBJECT_LINE.match(line)
            if m:
                names.add(m.group(2))
    return names


def align_text_2(elf: bytes) -> bytes:
    """elf (a 32-bit little-endian object) with .text sh_addralign 2."""
    data = bytearray(elf)
    shoff, = struct.unpack_from("<I", data, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from("<HHH", data, 0x2E)
    strtab, = struct.unpack_from("<I", data, shoff + shstrndx * shentsize + 0x10)
    found = False
    for i in range(shnum):
        sh = shoff + i * shentsize
        name_at = strtab + struct.unpack_from("<I", data, sh)[0]
        if data[name_at:data.index(0, name_at)] == b".text":
            struct.pack_into("<I", data, sh + 0x20, 2)
            found = True
    if not found:
        raise ValueError("no .text section")
    return bytes(data)


def copy_objects(lcf: str, objects: list[str], out_dir: Path) -> list[str]:
    """objects with each marked one replaced by a copy under out_dir (same
    path below build/), written with .text alignment 2."""
    names = marked_names(lcf)
    out = []
    for path in objects:
        p = Path(path)
        if p.name not in names:
            out.append(path)
            continue
        dest = out_dir / Path(*p.parts[1:])
        dest.parent.mkdir(parents=True, exist_ok=True)
        new = align_text_2(p.read_bytes())
        if not dest.exists() or dest.read_bytes() != new:
            dest.write_bytes(new)
        out.append(dest.as_posix())
    return out


def main() -> int:
    cmd, *args = sys.argv[1:]
    if cmd == "lcf":
        lcf, delinks = (Path(a) for a in args[:2])
        blocks = tuple(args[2:]) or ("ARM9",)
        text = lcf.read_text()
        try:
            new = mark(text, delinks.read_text(), blocks)
        except ValueError as e:
            print(f"lcf_align2.py: {e}", file=sys.stderr)
            return 1
        if new != text:
            lcf.write_text(new)
        return 0
    if cmd == "objects":
        lcf, objects, out_objects, out_dir = (Path(a) for a in args)
        paths = [l.strip().strip('"') for l in objects.read_text().splitlines()]
        paths = [p for p in paths if p]
        new = copy_objects(lcf.read_text(), paths, out_dir)
        out_objects.write_text("".join(f'"{p}"\n' for p in new))
        return 0
    print(__doc__, file=sys.stderr)
    return 2


if __name__ == "__main__":
    sys.exit(main())
