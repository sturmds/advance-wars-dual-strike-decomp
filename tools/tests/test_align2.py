#!/usr/bin/env python3
"""Check tools/lcf_align2.py (objects that start at 2 mod 4).

    python3 tools/tests/test_align2.py

- `mark` wraps each object that starts at 2 mod 4 in ALIGNALL(2) and
  ALIGNALL(4), joins neighbours into one block, leaves other objects and
  other sections alone, and fails on a name not in the text list.
- `copy_objects` copies only the marked objects, with .text alignment 2.

No ROM, build or compiler needed.
"""

import os
import struct
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
import lcf_align2  # noqa: E402

failures = []


def expect(what: str, got, want) -> None:
    print(f"{what}: {'ok' if got == want else 'FAIL'}")
    if got != want:
        failures.append(what)
        print(f"  got:  {got!r}\n  want: {want!r}")


def lcf(*text: str) -> str:
    """An .arm9 block shaped like dsd lcf's, with one .rodata line."""
    lines = ["    .arm9 : {", "        ALIGNALL(4);", "        ARM9_TEXT_START = .;"]
    lines += [f"        {t}" for t in text]
    lines += ["        ARM9_TEXT_END = .;", "        b.o(.rodata)", "    }", ""]
    return "\n".join(lines)


# b starts at 2 mod 4; c ends at 2 mod 4 (a 6-byte stub).
DELINKS = """    .text       start:0x02000000 end:0x02001000 kind:code align:32

src/sdk/a.c:
    complete
    .text       start:0x02000010 end:0x02000014

src/sdk/b.c:
    complete
    .text       start:0x02000036 end:0x0200003a

src/sdk/c.c:
    complete
    .text       start:0x02000050 end:0x02000056

src/sdk/d.c:
    complete
    .text       start:0x02000060 end:0x02000064
"""

base = lcf("gap_1.o(.text)", "a.o(.text)", "gap_2.o(.text)", "b.o(.text)", "gap_3.o(.text)",
           "c.o(.text)", "gap_4.o(.text)", "d.o(.text)", "gap_5.o(.text)")
want = lcf("gap_1.o(.text)", "a.o(.text)", "gap_2.o(.text)",
           "ALIGNALL(2);", "b.o(.text)", "gap_3.o(.text)", "ALIGNALL(4);",
           "c.o(.text)", "ALIGNALL(2);", "gap_4.o(.text)", "ALIGNALL(4);",
           "d.o(.text)", "gap_5.o(.text)")
marked = lcf_align2.mark(base, DELINKS)
expect("mark: start and end at 2 mod 4", marked, want)
expect("mark: second run changes nothing", lcf_align2.mark(marked, DELINKS), marked)
aligned = (DELINKS.replace("0x02000036", "0x02000034").replace("0x0200003a", "0x02000038")
           .replace("0x02000056", "0x02000058"))
expect("mark: nothing at 2 mod 4", lcf_align2.mark(base, aligned), base)
expect("marked_names", lcf_align2.marked_names(marked), {"b.o", "gap_3.o", "gap_4.o"})
try:
    lcf_align2.mark(lcf("gap_1.o(.text)"), DELINKS)
    expect("mark: missing object raises", False, True)
except ValueError as e:
    expect("mark: missing object raises", "b.o" in str(e), True)


def elf(align: int) -> bytes:
    """A minimal ELF32 object with sections null, .text, .shstrtab."""
    names = b"\0.text\0.shstrtab\0"
    header = bytearray(0x34)
    header[:5] = b"\x7fELF\x01"
    struct.pack_into("<I", header, 0x20, 0x34 + len(names))
    struct.pack_into("<HHH", header, 0x2E, 40, 3, 2)
    sections = bytearray(40 * 3)
    struct.pack_into("<10I", sections, 40, 1, 1, 6, 0, 0, 0, 0, 0, align, 0)
    struct.pack_into("<10I", sections, 80, 7, 3, 0, 0, 0x34, len(names), 0, 0, 1, 0)
    return bytes(header) + names + bytes(sections)


expect("align_text_2", lcf_align2.align_text_2(elf(4)), elf(2))

with tempfile.TemporaryDirectory() as tmp:
    t = Path(tmp)
    (t / "build/src").mkdir(parents=True)
    (t / "build/delinks").mkdir()
    for p in ("build/src/a.o", "build/src/b.o", "build/delinks/gap_3.o"):
        (t / p).write_bytes(elf(4))
    objects = ["build/src/a.o", "build/src/b.o", "build/delinks/gap_3.o"]
    here = os.getcwd()
    os.chdir(t)
    try:
        out = lcf_align2.copy_objects(marked, objects, Path("build/align2"))
    finally:
        os.chdir(here)
    expect("copy: paths", out, ["build/src/a.o", "build/align2/src/b.o", "build/align2/delinks/gap_3.o"])
    expect("copy: alignment 2", (t / out[1]).read_bytes(), elf(2))
    expect("copy: original unchanged", (t / "build/src/b.o").read_bytes(), elf(4))

if failures:
    print(f"{len(failures)} failed")
    sys.exit(1)
print("all passed")
