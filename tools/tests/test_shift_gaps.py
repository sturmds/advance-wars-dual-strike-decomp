#!/usr/bin/env python3
"""Check which C text tools/shift_gaps.py counts as a gap, on small samples.

    python3 tools/tests/test_shift_gaps.py

No build or ROM needed.
"""

import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
import shift_gaps  # noqa: E402

errors = []


def expect(name: str, text: str, want: list[str]) -> None:
    got = [kind for kind, _, _ in shift_gaps.source_sites("src/game/main/x.c", text)]
    if got != want:
        errors.append(f"{name}: got {got}, want {want}")
    else:
        print(f"{name}: ok")


expect("prototype is not a call", "void Mem_Copy32(const void* src, void* dst, u32 size);\n", [])
expect("literal copy length", "void f(void) {\n  Mem_Copy32(a, b, 0x40);\n}\n", ["copy"])
expect("sizeof copy length", "void f(void) {\n  Mem_Copy32(a, b, sizeof(*b));\n}\n", [])
expect("Mem_CopyUnits length", "void f(void) {\n  Mem_CopyUnits(a, b, 4, 16);\n}\n", ["copy"])
expect("Mem_CopyUnits unit bits", "void f(void) {\n  Mem_CopyUnits(a, b, sizeof(*a), 16);\n}\n", [])
expect("literal expression", "void f(void) {\n  memset(a, 0, (4 * 0x10));\n}\n", ["copy"])
expect("literal alloc", "void f(void) {\n  p = Mem_Alloc(0x2f4);\n}\n", ["alloc"])
expect("sizeof alloc", "void f(void) {\n  p = Mem_Alloc(sizeof(Work));\n}\n", [])
expect("byte offset", "void f(void) {\n  g((u8*)&gMenuState + 0x1000);\n}\n", ["offset"])
expect("literal bound", "void f(void) {\n  for (i = 0; i < 6; i++) {\n  }\n}\n", ["loop"])
expect("count-down to 0", "void f(void) {\n  for (i = 5; i >= 0; i--) {\n  }\n}\n", [])
expect("named bound", "void f(void) {\n  for (i = 0; i < CO_COUNT; i++) {\n  }\n}\n", [])
expect("u32 table", 'const u32 data_x[12] = {\n#include "data/main/data_x.inc.c"\n};\n', ["array_u32"])
expect("char table", 'char data_x[8] = {\n#include "data/main/data_x.inc.c"\n};\n', ["array_char"])
expect("struct table", 'MenuItem data_x[3] = {\n#include "data/main/data_x.inc.c"\n};\n', ["array_other"])
expect("table sized by its initializer", 'u32 data_x[] = {\n#include "data/main/data_x.inc.c"\n};\n', [])
expect("local array", "void f(void) {\n  u8 buf[4] = {0};\n}\n", [])
expect("extern with a length", "extern u16 data_x[0x20];\n", ["extern"])
expect("extern without one", "extern u16 data_x[];\n", [])
expect("placeholder", "u8 data_x[0x40];\n", ["placeholder"])
expect("struct field is no placeholder", "typedef struct {\n  u8 name[0x40];\n} X;\n", [])
expect("comment ignored", "/* Mem_Alloc(4) and for (i = 0; i < 3; i++) */\n", [])
expect("size assert", "typedef char X_size_check[sizeof(X) == 4 ? 1 : -1];\n", ["size_assert"])

with tempfile.TemporaryDirectory(prefix="test_shift_gaps") as d:
    aliases = Path(d) / "aliases.txt"
    aliases.write_text("main a = b+0x4 real [1]\nmain c = b+0x100 real +0x100\nmain e = b+0x6 real [1].x+0x2\n")
    got = [n for _, _, n, _ in shift_gaps.alias_sites(aliases)]
    if got != [2, 3]:
        errors.append(f"alias: got lines {got}, want [2, 3] (a byte count, or a path that ends in one)")
    else:
        print("alias: ok")

if shift_gaps.area("src/game/main/x.c") != "game" or shift_gaps.area("src/sdk/os/x.c") != "SDK":
    errors.append("area")


def extern_types_tree(root: Path) -> None:
    """A small tree for extern_types.py: definitions, readers and a header."""
    (root / "include/game").mkdir(parents=True)
    (root / "src/game").mkdir(parents=True)
    (root / "include/types.h").write_text("typedef unsigned char u8;\ntypedef unsigned int u32;\n")
    (root / "include/game/foo.h").write_text(
        "#ifndef FOO_H\n#define FOO_H\n\n#include \"types.h\"\n\n#define FOO_ROWS 4\n\n"
        "typedef struct Foo {\n  u32 a;\n} Foo;\n\nextern Foo gFoo;\n\n#endif\n")
    (root / "src/game/defs.c").write_text(
        '#include "game/foo.h"\n\nFoo gFoo;\nconst Foo data_table[] = {\n#include "x.inc.c"\n};\n'
        "unsigned char data_bytes[8];\nu32 data_grid[3][FOO_ROWS];\nvoid f(void) {\n  u8 local[2];\n}\n")
    (root / "src/game/reader.c").write_text(
        '#include "types.h"\n\nextern u8 data_table[]; /* rows */\nextern u8 data_bytes[];\n'
        "extern u32 data_grid[][4];\nextern u8\n    gFoo;\nextern u8 not_defined[];\n\n"
        "void g(void) {}\n")
    (root / "src/game/copy.c").write_text(
        '#include "types.h"\n\ntypedef struct Foo {\n  u32 a;\n} Foo;\n\nextern Foo gFoo;\n')


with tempfile.TemporaryDirectory(prefix="test_extern_types") as d:
    import extern_types  # noqa: E402

    root = Path(d)
    extern_types_tree(root)
    extern_types.ROOT = root
    for f in (extern_types.collect, extern_types.defines, extern_types.type_headers, extern_types.file_types):
        f.cache_clear()
    got = [(e.name, e.line) for e, _ in extern_types.mismatches()]
    # data_bytes agrees (unsigned char is u8), data_grid agrees (FOO_ROWS is 4),
    # not_defined has no definition; copy.c declares gFoo with a Foo of its own.
    if got != [("gFoo", 7), ("data_table", 3), ("gFoo", 6)]:
        errors.append(f"extern_types: got {got}, want gFoo in copy.c, data_table and gFoo in reader.c")
    else:
        print("extern_types differ: ok")
    done, left = extern_types.fix(None)
    text = (root / "src/game/reader.c").read_text()
    want = ('#include "types.h"\n#include "game/foo.h"\n\nextern const Foo data_table[]; /* rows */\n'
            "extern u8 data_bytes[];\nextern u32 data_grid[][4];\nextern Foo gFoo;\n")
    if done != 2 or len(left) != 1 or not text.startswith(want):
        errors.append(f"extern_types --fix: {done} rewritten, left {left}, text:\n{text}")
    else:
        print("extern_types --fix: ok")
    extern_types.collect.cache_clear()
    extern_types.file_types.cache_clear()
    if [e.path for e, _ in extern_types.mismatches()] != ["src/game/copy.c"]:
        errors.append("extern_types: reader.c differs after --fix")
    errs = extern_types.check()
    if len(errs) != 1 or "copy.c" not in errs[0]:
        errors.append(f"extern_types --check: want one error for copy.c, got {errs}")
    (root / "config/AWRE").mkdir(parents=True)
    (root / "config/AWRE/extern_types.txt").write_text(
        "# comment\nsrc/game/copy.c gFoo a local copy kept on purpose\nsrc/game/reader.c gFoo agrees now\n")
    errs = extern_types.check()
    if extern_types.mismatches() or len(errs) != 1 or "reader.c gFoo" not in errs[0]:
        errors.append(f"extern_types: a listed extern is left out and a stale entry reported, got {errs}")
    else:
        print("extern_types --check: ok")

if errors:
    sys.exit("test_shift_gaps: " + "\n  ".join(errors))
print("test_shift_gaps: ok")
