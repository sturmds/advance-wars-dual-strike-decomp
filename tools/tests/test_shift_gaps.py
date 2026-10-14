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

if errors:
    sys.exit("test_shift_gaps: " + "\n  ".join(errors))
print("test_shift_gaps: ok")
