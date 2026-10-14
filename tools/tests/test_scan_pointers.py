#!/usr/bin/env python3
"""Check tools/scan_pointers.py on made-up modules.

    python3 tools/tests/test_scan_pointers.py

- Levels: a value at a module end, at the end of the overlays or up to 0x20
  bytes past a boundary is `layout`; a FIXED value is no candidate; a literal
  pool word in main RAM outside every module is `medium`, a data word with
  the same value is no candidate.
- --unused-symbols: which symbols count as unused, and their group.
- The source file and holder of an address, and the module of a linked object.
- C types: a data word in a u8/u16 array, or in narrow fields of a struct
  (from a dsindex record or a typedef in an included build/gen file), is a
  number; a u32 array, a pointer, or a word that reaches a pointer field or
  the next object is not. Objects of two overlays at one address stay apart.
- The list of reviewed numbers.

No ROM or build needed.
"""

import json
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
import modmap  # noqa: E402
import scan_pointers as sp  # noqa: E402

failures = []


def expect(what: str, got, want) -> None:
    print(f"{what}: {'ok' if got == want else 'FAIL'}")
    if got != want:
        print(f"  got  {got!r}\n  want {want!r}")
        failures.append(what)


def module(name, sections, symbols=(), files=()):
    secs = [modmap.Section(n, s, e, k) for n, s, e, k in sections]
    m = modmap.Module(name, Path("."), secs, bytes(secs[-1].end - secs[0].start))
    m.symbols = [modmap.Symbol(n, k, a, "") for n, k, a in symbols]
    m.files = sorted(files)
    return m


main = module("main", [(".text", 0x02000000, 0x02001000, "code"), (".data", 0x02001000, 0x02001100, "data"),
                       (".bss", 0x02001100, 0x02002000, "bss")],
              [("Func", "function(arm,size=0x10)", 0x02000100), ("data_02001040", "data(byte[16])", 0x02001040)],
              [(0x02000100, 0x02000110, "src/func.c"), (0x02001040, 0x02001050, "src/data.c")])
ov000 = module("ov000", [(".text", 0x02002000, 0x02002800, "code"), (".bss", 0x02002800, 0x02003000, "bss")])
ov001 = module("ov001", [(".text", 0x02002000, 0x02004000, "code")])
mods = {m.name: m for m in (main, ov000, ov001)}
levels = sp.Levels(mods)


# Levels
expect("end of the overlays", levels.level(0x02004000, True, False), ("layout", "end of the overlays"))
expect("module end + 0x10", levels.level(0x02003010, False, False), ("layout", "ov000 .bss end + 0x10"))
expect("0x21 past the end of the overlays", levels.level(0x02004021, False, False), None)
expect("section start, before the in-module test", levels.level(0x02001100, False, True)[0], "layout")
expect("FIXED value", levels.level(0x023fe800, True, False), None)
expect("pool word in main RAM, no module", levels.level(0x02300000, True, False), ("medium", "main RAM, no module"))
expect("data word in main RAM, no module", levels.level(0x02300000, False, False), None)
expect("pool word in DTCM", levels.level(0x027c3800, True, False), ("medium", "dtcm, no module"))
expect("symbol start with a relocated neighbour", levels.level(0x02001040, False, True), ("high", "main"))
expect("symbol start alone", levels.level(0x02001040, False, False), ("medium", "main"))
expect("aligned with a relocated neighbour", levels.level(0x02001060, False, True), ("medium", "main"))
expect("anything else in range", levels.level(0x02001063, False, False), ("low", "main"))


# Unused symbols
def sym(name, kind, addr):
    return modmap.Symbol(name, kind, addr, "")


symbols = [("main", s) for s in (
    sym("Func", "function(arm,size=0x10)", 0x02000100),
    sym("Table", "data(byte[64])", 0x02001000),
    sym("data_02001006", "data(any)", 0x02001006),       # inside Table, nothing uses it
    sym("data_02001008", "data(any)", 0x02001008),       # inside Table, a relocation targets it
    sym("data_0200100a", "data(any)", 0x0200100a),       # inside Table, C names it
    sym("data_02001040", "data(byte[16])", 0x02001040),  # an object nobody refers to
    sym("data_02001050", "data(any)", 0x02001050),       # after every object, in no linker script
    sym("Alias", "data(any)", 0x02001050),                # the linker script defines it
)]
rows = sp.unused_symbols(symbols, targets={("main", 0x02001008)}, relocated={"Table"}, named={"data_0200100a"},
                         defined={"Table", "data_02001040", "Func"}, lcf={"Alias"})
expect("unused symbols and groups", sorted((s.name, g) for _, s, g in rows),
       [("Alias", "alias"), ("data_02001006", "alias"), ("data_02001040", "object")])
expect("declared size of byte[N]", sp.declared_size("data(byte[1084])"), 1084)
expect("declared size of bss", sp.declared_size("bss(size=0xfe70)"), 0xfe70)
expect("declared size of any", sp.declared_size("data(any)"), 0)


# Holders and object sources
with tempfile.TemporaryDirectory() as tmp:
    index = Path(tmp) / "fnindex.json"
    index.write_text(json.dumps({"functions": [{"name": "Func", "module": "main", "addr": 0x02000100,
                                                "size": 0x10, "pool": 0xc, "file": "src/func.c"}]}))
    holders = sp.Holders(mods, index)
    expect("pool word holder", holders.holder("main", 0x0200010c, True), ("Func+0xc", "src/func.c"))
    expect("data word holder", holders.holder("main", 0x02001048, False), ("data_02001040+0x8", "src/data.c"))
    expect("address in no file", holders.file("main", 0x02001080), "")
expect("object of a C file", sp.object_source("build/align2/src/game/x.o"), "src/game/x.c")
expect("object of an ARM7 C file", sp.object_source("build/arm7/src/arm7/sdk/y.o"), "src/arm7/sdk/y.c")
expect("gap object", sp.object_source("build/delinks/_dsd_gap@main_12.o"), "main")


# C types
with tempfile.TemporaryDirectory() as tmp:
    root = Path(tmp)
    (root / "src").mkdir()
    (root / "build/gen/maps").mkdir(parents=True)
    (root / "src/a.c").write_text('''#include "types.h"
#include "maps/gen.h"
/* u8 Comment[4] = { */
const u8 Bytes[16] = {
#include "data/Bytes.inc.c"
};
u32 Words[4] = {0};
void *Ptrs[2] = {0};
Rec Recs[2] = {0};
static const Mixed MixedOne = {0};
GenTiles Tiles = {0};
''')
    (root / "build/gen/maps/gen.h").write_text(
        "typedef struct GenTiles {\n    u8 m001[0x10]; /* tiles */\n    u16 m002[4];\n} GenTiles;\n")
    structs = sp.struct_table({"headers": {"include/rec.h": {"structs": [
        {"name": "Rec", "kind": "struct", "size": 8, "fields": [
            {"name": "x", "offset": 0, "size": 1, "type": "u8"},
            {"name": "pad", "offset": 1, "size": 1, "type": "u8[1]", "count": 1, "elem_size": 1},
            {"name": "y", "offset": 2, "size": 2, "type": "s16"},
            {"name": "inner", "offset": 4, "size": 4, "type": "Pair", "struct": "Pair"}]},
        {"name": "Pair", "kind": "struct", "size": 4, "fields": [
            {"name": "a", "offset": 0, "size": 2, "type": "u16[2]", "count": 2, "elem_size": 2}]},
        {"name": "Mixed", "kind": "struct", "size": 12, "fields": [
            {"name": "a", "offset": 0, "size": 2, "type": "u16"},
            {"name": "p", "offset": 4, "size": 4, "type": "char *"},
            {"name": "b", "offset": 8, "size": 4, "type": "u8[4]", "count": 4, "elem_size": 1}]}]}}})
    objects = [("main", 0x100, 16, "Bytes", "src/a.c"), ("main", 0x110, 16, "Words", "src/a.c"),
               ("main", 0x120, 8, "Ptrs", "src/a.c"), ("main", 0x128, 16, "Recs", "src/a.c"),
               ("main", 0x138, 12, "MixedOne", "src/a.c"), ("main", 0x144, 24, "Tiles", "src/a.c"),
               ("ov001", 0x100, 16, "Words", "src/a.c")]
    saved_root = modmap.ROOT
    modmap.ROOT = root
    try:
        types = sp.DataTypes(objects, structs)
        expect("u8 array", types.number("main", 0x104), True)
        expect("word past the end of its object", types.number("main", 0x10e), False)
        expect("u32 array", types.number("main", 0x114), False)
        expect("pointer array", types.number("main", 0x120), False)
        expect("struct of narrow fields, second element", types.number("main", 0x130), True)
        expect("struct across two elements", types.number("main", 0x12c), True)
        expect("struct, word with a pointer field", types.number("main", 0x13a), False)
        expect("struct, narrow field after a pointer", types.number("main", 0x140), True)
        expect("narrow typedef from build/gen", types.number("main", 0x150), True)
        expect("same address, other overlay", types.number("ov001", 0x104), False)
        expect("address in no object", types.number("main", 0x200), False)
    finally:
        modmap.ROOT = saved_root

    numbers = Path(tmp) / "numbers.txt"
    numbers.write_text("# comment\n\n0x02000b10  BuildInfo[4]\n0x0213825c  halfword pairs, same run\n")
    expect("reviewed numbers", sp.reviewed_numbers(numbers),
           {0x02000b10: "BuildInfo[4]", 0x0213825c: "halfword pairs, same run"})

if failures:
    print(f"{len(failures)} failed: {', '.join(failures)}")
    sys.exit(1)
print("test_scan_pointers: all ok")
