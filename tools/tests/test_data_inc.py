#!/usr/bin/env python3
"""Check tools/data_inc.py and include/text_data.h.

    python3 tools/tests/test_data_inc.py

- Rows of data_inc.txt parse, and bad ones stop the tool.
- render() writes plain values as hex or decimal by type, rows in braces for
  `type[N]`, relocated words as symbols (with an offset when the target is
  inside a symbol, or a layout symbol of that value), relocs.txt's `add:`,
  and stops on a pointer without a relocation.
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
rom = FakeRom(words(0x1101), {0x1100: "ThumbFunc"}, {0x1000: ("load", 0x1101, "itcm")})
rom.thumb["itcm"] = {0x1100}
expect("render: Thumb function", body(data_inc.render(data_inc.Row("itcm gT 0x1000 4 void*", "t"), rom)),
       "(void*)&ThumbFunc,\n")
rom = FakeRom(words(0x0102, 0x1208, 0x1100, 0x0304, 0, 0), {0x1100: "FuncA", 0x1200: "gData"},
              {0x1004: ("load", 0x1208, "itcm")})
expect("render: structs", body(data_inc.render(data_inc.Row("itcm gS 0x1000 0x18 {u8,u8,u16,void*,u8[4]}", "t"), rom)),
       "0x02, 0x01, 0x0000, (void*)((char*)&gData + 0x8), 0x00, 0x11, 0x00, 0x00,\n"
       "0x04, 0x03, 0x0000, 0, 0x00, 0x00, 0x00, 0x00,\n")
expect("render: an array first, without braces",
       body(data_inc.render(data_inc.Row("itcm gS 0x1000 8 {u16[2],u32}", "t"), rom)),
       "0x0102, 0x0000, (u32)((char*)&gData + 0x8),\n")
rom = FakeRom(bytes([1, 0, 0, 0, 2, 0, 0, 0]), {}, {})
expect("render: padding", body(data_inc.render(data_inc.Row("itcm gS 0x1000 8 {u8,pad[3],u32}", "t"), rom)),
       "0x01, 0x00000002,\n")
rom = FakeRom(bytes([1, 9, 0, 0, 2, 0, 0, 0]), {}, {})
stops("render: padding that is not zero",
      lambda: data_inc.render(data_inc.Row("itcm gS 0x1000 8 {u8,pad[3],u32}", "t"), rom))
rom = FakeRom(bytes([0x21, 0x43, 0xf5, 0x07]), {}, {})
expect("render: bit fields from the low end, signed ones too",
       body(data_inc.render(data_inc.Row("itcm gS 0x1000 4 {u8:4,u8:4,u8:4,u8:4,s8:3,s8:5,u8}", "t"), rom)),
       "0x1, 0x2, 0x3, 0x4, -3, -2, 0x07,\n")
rom = FakeRom(bytes([0x0f, 0x2a]), {}, {})
expect("render: a bit field that does not fit opens a new unit",
       body(data_inc.render(data_inc.Row("itcm gS 0x1000 2 {u8:4,u8:6}", "t"), rom)), "0xf, 0x2a,\n")
rom = FakeRom(words(0x0b, 0x05), {}, {})
expect("render: a :0 field ends the unit and takes no value",
       body(data_inc.render(data_inc.Row("itcm gS 0x1000 8 {u32:2,u32:3,u32:0,u32:3}", "t"), rom)),
       "0x3, 0x2, 0x5,\n")
rom = FakeRom(words(0, 0xaa000000, 0x00010008), {}, {})
expect("render: a u64 field, word aligned, with ULL",
       body(data_inc.render(data_inc.Row("itcm gS 0x1000 12 {u64,u16,u16}", "t"), rom)),
       "0xaa00000000000000ULL, 0x0008, 0x0001,\n")
stops("row: a :0 field with a count", lambda: data_inc.Row("itcm gS 0x1000 4 {u32:0[2],u32}", "t"))
stops("row: bit field wider than its type", lambda: data_inc.Row("itcm gS 0x1000 1 {u8:9}", "t"))
stops("row: struct field not aligned", lambda: data_inc.Row("itcm gS 0x1000 8 {u8,u32,u8[3]}", "t"))
stops("row: size not whole structs", lambda: data_inc.Row("itcm gS 0x1000 6 {u32}", "t"))
rom = FakeRom(words(0x1100), {}, {})
stops("render: pointer without a relocation", lambda: data_inc.render(data_inc.Row("itcm gP 0x1000 4 void*", "t"), rom))
rom = FakeRom(words(0x02175180, 0x02175184), {0x02173788: "gPool"},
              {0x1000: ("load", 0x02175180, "itcm"), 0x1004: ("load", 0x02175184, "itcm")})
expect("render: a layout symbol's value inside a symbol",
       body(data_inc.render(data_inc.Row("itcm gL 0x1000 8 u32", "t"), rom)),
       "(u32)&AUTOLOAD_LIST_START, (u32)((char*)&gPool + 0x19fc),\n")
with tempfile.TemporaryDirectory() as tmp:
    (Path(tmp) / "relocs.txt").write_text("from:0x00001000 kind:load to:0x00001100 add:0x8 module:itcm\n")
    saved = data_inc.config_dir
    data_inc.config_dir = lambda module: Path(tmp)
    try:
        expect("relocs: add is part of the target", data_inc.Rom().reloc_table("itcm"),
               {0x1000: ("load", 0x1108, "itcm")})
    finally:
        data_inc.config_dir = saved
rom = FakeRom(words(0x1100), {}, {0x1000: ("arm_call", 0x1100, "itcm")})
stops("render: relocation that is not a data word", lambda: data_inc.render(data_inc.Row("itcm gP 0x1000 4 void*", "t"), rom))

# Values named by #define (the prefix's names from a header made here)
with tempfile.TemporaryDirectory() as tmp:
    (Path(tmp) / "game").mkdir()
    (Path(tmp) / "game" / "k.h").write_text("#define TKIND_A 1\n#define TKIND_B \\\n  2\n#define TKIND_C 3\n"
                                            "#define TKIND_D 3\n#define TKINDX_E 4\n")
    saved_root, saved_cache = data_inc.ROOT, dict(data_inc._constants)
    data_inc.ROOT, data_inc._constants = Path(tmp), {}
    (Path(tmp) / "include").mkdir()
    (Path(tmp) / "game").rename(Path(tmp) / "include" / "game")
    try:
        rom = FakeRom(bytes([1, 0, 2, 0, 3, 0, 4, 0, 5, 0]), {}, {})
        named = data_inc.Row("itcm gK 0x1000 10 {u8:TKIND,u8}", "t")
        expect("names: one name per value, numbers otherwise", body(data_inc.render(named, rom)),
               "TKIND_A, 0x00,\nTKIND_B, 0x00,\n0x03, 0x00,\n0x04, 0x00,\n0x05, 0x00,\n")
        expect("names: the header is an input", [f for f in data_inc.config_inputs([named]) if f.startswith("include")],
               ["include/game/k.h"])
    finally:
        data_inc.ROOT, data_inc._constants = saved_root, saved_cache
stops("row: names on a pointer field", lambda: data_inc.Row("itcm gK 0x1000 4 {void*:TKIND}", "t"))

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
