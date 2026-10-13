#!/usr/bin/env python3
"""Check tools/data_inc.py and include/text_data.h.

    python3 tools/tests/test_data_inc.py

- Rows of data_inc.txt parse, and bad ones stop the tool.
- render() writes plain values as hex or decimal by type, rows in braces for
  `type[N]`, relocated words as symbols (with an offset when the target is
  inside a symbol), and stops on a pointer without a relocation.
- A definition between `#pragma section TEXT_DATA begin` and `end` compiles
  into a section named .text with its relocations, one section per
  definition, smallest first.

Uses made-up bytes and the compiler from make setup; no ROM or build needed.
"""

import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
import data_inc  # noqa: E402
import fnindex  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent.parent
failures = []


def expect(what: str, got, want) -> None:
    print(f"{what}: {'ok' if got == want else 'FAIL'}")
    if got != want:
        print(f"  got:  {got!r}\n  want: {want!r}")
        failures.append(what)


def stops(what: str, fn) -> None:
    try:
        fn()
    except SystemExit as e:
        expect(what, isinstance(e.code, str) and e.code.startswith("error:"), True)
        return
    expect(what, "returned", "stopped")


class FakeRom(data_inc.Rom):
    """One made-up module, `itcm` at 0x1000."""

    def __init__(self, data: bytes, symbols: dict[int, str], relocs: dict[int, tuple[str, int, str]]) -> None:
        super().__init__()
        self.images["itcm"] = (0x1000, data)
        self.symbols["itcm"] = symbols
        self.relocs["itcm"] = relocs


def words(*values: int) -> bytes:
    return b"".join(v.to_bytes(4, "little") for v in values)


def body(text: str) -> str:
    """render() output without its comment line."""
    return text.split("\n", 1)[1]


# Rows
row = data_inc.Row("itcm gTable 0x1000 0x18 u32[3]", "test")
expect("row: fields", (row.module, row.symbol, row.addr, row.size, row.type, row.columns),
       ("itcm", "gTable", 0x1000, 0x18, "u32", 3))
expect("row: pointer type", data_inc.Row("itcm gPtrs 0x1000 8 void*", "test").pointer, True)
stops("row: unknown type", lambda: data_inc.Row("itcm gX 0x1000 4 float", "test"))
stops("row: size not a multiple of the element", lambda: data_inc.Row("itcm gX 0x1000 6 u32", "test"))
stops("row: elements do not fill the rows", lambda: data_inc.Row("itcm gX 0x1000 0x10 u32[3]", "test"))

# Values
rom = FakeRom(words(1, 2, 3, 4, 5, 6), {0x1000: "gTable"}, {})
expect("render: rows of 3", body(data_inc.render(row, rom)),
       "{0x00000001, 0x00000002, 0x00000003},\n{0x00000004, 0x00000005, 0x00000006},\n")
rom = FakeRom(bytes([0xff, 0xff, 0x02, 0x00]), {}, {})
expect("render: s16 in decimal", body(data_inc.render(data_inc.Row("itcm gS 0x1000 4 s16", "t"), rom)), "-1, 2,\n")
expect("render: u8 in hex", body(data_inc.render(data_inc.Row("itcm gB 0x1000 2 u8", "t"), rom)), "0xff, 0xff,\n")

# Relocations
rom = FakeRom(words(0x1100, 0, 0x1208),
              {0x1100: "FuncA", 0x1200: "gData"},
              {0x1000: ("load", 0x1100, "itcm"), 0x1008: ("load", 0x1208, "itcm")})
ptrs = data_inc.Row("itcm gPtrs 0x1000 0xc void*", "t")
expect("render: pointers as symbols", body(data_inc.render(ptrs, rom)),
       "(void*)&FuncA, 0, (void*)((char*)&gData + 0x8),\n")
expect("render: relocated u32", body(data_inc.render(data_inc.Row("itcm gW 0x1000 4 u32", "t"), rom)),
       "(u32)&FuncA,\n")
rom = FakeRom(words(0x1100), {}, {})
stops("render: pointer without a relocation", lambda: data_inc.render(data_inc.Row("itcm gP 0x1000 4 void*", "t"), rom))
rom = FakeRom(words(0x1100), {}, {0x1000: ("arm_call", 0x1100, "itcm")})
stops("render: relocation that is not a data word", lambda: data_inc.render(data_inc.Row("itcm gP 0x1000 4 void*", "t"), rom))

# The section pragma, with the compiler
src = """#include "types.h"
#include "text_data.h"

void Handler(void);

#pragma section TEXT_DATA begin
u32 gFirst[2] = {1, 2};
void* gSecond[1] = {(void*)&Handler};
#pragma section TEXT_DATA end
"""
with tempfile.TemporaryDirectory() as tmp:
    c, o = Path(tmp) / "text_data_test.c", Path(tmp) / "text_data_test.o"
    c.write_text(src)
    res = fnindex.compile_c(c, o)
    expect("pragma: compiles", res.returncode, 0)
    if res.returncode == 0:
        sections, syms, relas = fnindex.read_elf(o.read_bytes())
        names = [n for n, _ in sections]
        placed = {s.name: names[s.shndx] for s in syms if s.name in ("gFirst", "gSecond")}
        expect("pragma: both in .text", placed, {"gFirst": ".text", "gSecond": ".text"})
        order = sorted((s.shndx, s.name) for s in syms if s.name in ("gFirst", "gSecond"))
        # mwcc emits the definitions smallest first, not in source order.
        expect("pragma: smallest first", [n for _, n in order], ["gSecond", "gFirst"])
        second = next(s.shndx for s in syms if s.name == "gSecond")
        expect("pragma: relocation to Handler", len(relas.get(second, [])), 1)

sys.exit(1 if failures else 0)
