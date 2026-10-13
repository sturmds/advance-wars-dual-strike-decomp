#!/usr/bin/env python3
"""Checks mwcc_order.mwcc_heapsort against mwccarm: the order in which the
compiler emits the data objects of one file (docs/workflow.md, "Several data
objects in one file").

    python3 tools/tests/test_data_order.py

Compiles files of u8 arrays with sizes that repeat and reads the section
order from the object: initialized arrays, uninitialized ones (.bss), and
a mix of both kinds with `static` and zero-initialized ones. Also checks
that mwcc_order.definition_order gives a file that the compiler emits in
size order, for each kind. Runs every case with the ARM9 flags and again
with the ARM7 ones (`-O4,s -proc arm7tdmi`), since tools/bss.py uses the
model for both. Needs the compiler from `make setup`; skipped without it.
"""

import random
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
import configure  # noqa: E402
import mwcc_order  # noqa: E402
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


# Definition forms, and whether the compiler counts each as initialized.
FORMS = {"init": ("unsigned char v%d[%d] = {1};", True), "static_init": ("static unsigned char v%d[%d] = {1};", True),
         "zero": ("unsigned char v%d[%d] = {0};", True), "bss": ("unsigned char v%d[%d];", False),
         "static_bss": ("static unsigned char v%d[%d];", False)}


def compile_sizes(work: Path, flags: str, sizes: list[int], forms: list[str] | None = None) -> list[int]:
    c, o = work / "t.c", work / "t.o"
    forms = forms or ["init"] * len(sizes)
    text = "".join(FORMS[f][0] % (i, n) + "\n" for i, (f, n) in enumerate(zip(forms, sizes)))
    # Uses each object, so that no static one is dropped.
    text += "void f(void) {\n" + "".join(f"  v{i}[0]++;\n" for i in range(len(sizes))) + "}\n"
    c.write_text(text)
    r = subprocess.run([str(pragmas.CC), "-c", *flags.split(), "-o", str(o), str(c)],
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
    mixed = []
    for n in (9, 40):
        mixed.append(([rng.choice((4, 8, 12, 16)) for _ in range(n)], [rng.choice(list(FORMS)) for _ in range(n)]))
    if configure.CC7 != configure.CC:
        sys.exit("test_data_order: ARM7 has its own compiler now; run the ARM7 cases with it")
    with tempfile.TemporaryDirectory() as d:
        work = Path(d)
        for cpu, cflags in (("ARM9", configure.CFLAGS), ("ARM7", configure.CFLAGS7)):
            for kind in ("init", "bss"):
                for sizes in cases:
                    forms = [kind] * len(sizes)
                    flags = [FORMS[f][1] for f in forms]
                    got = compile_sizes(work, cflags, sizes, forms)
                    model = mwcc_order.mwcc_heapsort(sizes, flags)
                    check(f"{cpu}: mwcc_heapsort on {len(sizes)} {kind} objects", got == model,
                          f"compiler {got}, model {model}")
                sizes = sorted(cases[-1])
                order = mwcc_order.definition_order(sizes, kind == "init")
                got = compile_sizes(work, cflags, [sizes[i] for i in order], [kind] * len(sizes))
                check(f"{cpu}: definition_order gives size order ({kind})",
                      [order[i] for i in got] == list(range(len(sizes))))
            for sizes, forms in mixed:
                got = compile_sizes(work, cflags, sizes, forms)
                model = mwcc_order.mwcc_heapsort(sizes, [FORMS[f][1] for f in forms])
                check(f"{cpu}: mwcc_heapsort on {len(sizes)} objects of mixed forms", got == model,
                      f"compiler {got}, model {model}")
    return 1 if failures else 0


if __name__ == "__main__":
    sys.exit(main())
