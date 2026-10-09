#!/usr/bin/env python3
"""Checks for the text handling in tools/propagate.py.

    python3 tools/tests/test_propagate.py

Covers splitting a C file into top-level pieces, finding what a declaration
declares, renaming symbols, and writing a near twin's values. Needs no
build and no ROM.
"""

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
import propagate as P  # noqa: E402

SRC = """#include "types.h"
#define SCREEN (data_027c0288.unk_00)

/* a comment with a ; and { */
typedef struct Foo { int a; u8 b : 4; } Foo;
extern const void *const data_02152b78[][sizeof(Foo) / sizeof(void *)];
extern Foo *gFoo;
static int sCount = 3;
void Helper(int x);
u8 (*GetPicks(int mode))[2]
{
    return 0;
}

int Target(int i)
{
    Helper(gFoo->a + 0x40);
    return SCREEN + i * 4;
}
"""


def test_chunks():
    cs = P.chunks(P.strip_comments(SRC))
    kinds = [(c.kind, c.name if c.kind == "func" else None) for c in cs]
    assert kinds == [("pp", None), ("pp", None), ("decl", None), ("decl", None), ("decl", None),
                     ("decl", None), ("decl", None), ("func", "GetPicks"), ("func", "Target")], kinds
    assert cs[3].declared() == {"data_02152b78"}, cs[3].declared()
    assert cs[4].declared() == {"gFoo"}
    assert cs[5].defines_data and not cs[4].defines_data
    assert cs[6].declared() == {"Helper"}
    assert cs[2].is_type and cs[2].declared() == set()


def test_rename():
    text = "x = gFoo->gFoo + gFoo2; gFoo(); s.gFoo = 1;"
    out = P.rename(text, {"gFoo": "data_1", "gFoo2": "data_2"})
    assert out == "x = data_1->gFoo + data_2; data_1(); s.gFoo = 1;", out


def test_values():
    text = "p[0x10] = 4; q = x << 16; s.f : 8; r = -1; t = 0x40;"
    # 16 -> 32 in a shift stays (guarded); bit-field width stays.
    out = P.render_values("a : 8;\n" + text, {0x10: 0x20, 4: 5, -1: -2, 0x40: 0x80}, False, True, 0)
    assert out == "a : 8;\np[0x20] = 5; q = x << 16; s.f : 8; r = -2; t = 0x80;", out
    # Scaled: an index of 4-byte words follows a byte offset.
    out = P.render_values("w[3] = 1;", {12: 20}, True, True, 0)
    assert out == "w[5] = 1;", out
    # Values swap in one pass: 4 -> 8 and 8 -> 4 do not chain.
    out = P.render_values("a = 4; b = 8;", {4: 8, 8: 4}, False, True, 0)
    assert out == "a = 8; b = 4;", out
    # Unguarded shifts.
    out = P.render_values("q = x << 16;", {16: 8}, False, False, 0)
    assert out == "q = x << 8;", out


def main() -> int:
    for name, f in list(globals().items()):
        if name.startswith("test_"):
            f()
            print(f"ok {name}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
