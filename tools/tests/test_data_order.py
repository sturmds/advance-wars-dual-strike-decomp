#!/usr/bin/env python3
"""Checks maps.mwcc_heapsort against mwccarm: the order in which the compiler
emits the data objects of one file (docs/maps.md, "Build").

    python3 tools/tests/test_data_order.py

Compiles files of u8 arrays with sizes that repeat and reads the section
order from the object. Also checks that maps.definition_order gives a file
that the compiler emits in size order. Needs the compiler from `make setup`;
skipped without it.
"""

import random
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
import configure  # noqa: E402
import maps  # noqa: E402
import pragmas  # noqa: E402

ROOT = pragmas.ROOT
failures = []


def check(what: str, ok: bool, detail=""):
    print(f"{what}: {'ok' if ok else 'FAIL'}{f' ({detail})' if detail and not ok else ''}")
    if not ok:
        failures.append(what)


def emitted(obj: Path) -> list[int]:
    """The `vN` objects of an ELF file, by section index: the order the
    linker places them in."""
    d = obj.read_bytes()
    shoff, = struct.unpack_from("<I", d, 0x20)
    shentsize, shnum = struct.unpack_from("<HH", d, 0x2e)
    sections = [struct.unpack_from("<10I", d, shoff + i * shentsize) for i in range(shnum)]
    found = []
    for s in sections:
        if s[1] != 2:  # SHT_SYMTAB
            continue
        strings = sections[s[6]][4]
        for j in range(s[5] // 16):
            name, _, _, _, _, shndx = struct.unpack_from("<IIIBBH", d, s[4] + j * 16)
            text = d[strings + name:d.index(b"\0", strings + name)].decode()
            if text.startswith("v") and text[1:].isdigit():
                found.append((shndx, int(text[1:])))
    return [n for _, n in sorted(found)]


def compile_sizes(work: Path, sizes: list[int]) -> list[int]:
    c, o = work / "t.c", work / "t.o"
    c.write_text("".join(f"unsigned char v{i}[{n}] = {{1}};\n" for i, n in enumerate(sizes)))
    r = subprocess.run([str(pragmas.CC), "-c", *configure.CFLAGS.split(), "-o", str(o), str(c)],
                       cwd=ROOT, capture_output=True, text=True)
    if r.returncode:
        sys.exit(r.stdout + r.stderr)
    return emitted(o)


def main() -> int:
    if not pragmas.CC.exists():
        print("test_data_order: no compiler, run make setup (skipped)")
        return 0
    rng = random.Random(7)
    cases = [[13] * 10, [13, 13, 26, 13], [26, 13, 13, 13], [12, 4, 8, 12, 4],
             [rng.choice((13, 26, 39, 52)) for _ in range(60)]]
    with tempfile.TemporaryDirectory() as d:
        work = Path(d)
        for sizes in cases:
            got = compile_sizes(work, sizes)
            check(f"mwcc_heapsort on {len(sizes)} objects", got == maps.mwcc_heapsort(sizes),
                  f"compiler {got}, model {maps.mwcc_heapsort(sizes)}")
        sizes = sorted(cases[-1])
        order = maps.definition_order(sizes)
        got = compile_sizes(work, [sizes[i] for i in order])
        check("definition_order gives size order", [order[i] for i in got] == list(range(len(sizes))))
    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main())
