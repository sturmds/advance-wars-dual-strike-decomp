#!/usr/bin/env python3
"""Check tools/dsindex.py and tools/ds.py (issue 0041).

    python3 tools/tests/test_ds.py

1. A fixture header compiled with the pinned mwccarm: bit-field positions,
   unions, arrays, an unnamed struct member, an enum and a function pointer.
2. The load and store parser on fixed objdump lines.
3. Real headers: every `sizeof(X) == N` size check in include/ agrees with
   the size read from the DWARF. Nothing that renames jobs change.
4. ds.py dupes on a made-up index: same layouts, a local copy, overlapping
   union views, a symbol declared with two types.
5. With extract/ present, by address: the function at main 0x02095f28 reads
   offset 0xb (AiSite.score) with ldrb, and main 0x02153634 lies inside the
   CO table at 0x0215360c. Skipped without a ROM.

Needs the compiler from `make setup`; parts 1 and 3 are skipped without it.
"""

import re
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
import configure  # noqa: E402
import dsindex  # noqa: E402

ROOT = dsindex.ROOT
failures = []


def check(what: str, ok: bool, detail=""):
    print(f"{what}: {'ok' if ok else 'FAIL'}{f' ({detail})' if detail and not ok else ''}")
    if not ok:
        failures.append(what)


FIXTURE = """
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef struct Bits { u16 a : 3; u16 b : 5; s16 c : 8; } Bits;
typedef union Ref { struct { u16 index; u16 group; }; u32 id; } Ref;
typedef enum Kind { KIND_A, KIND_B = 5 } Kind;
typedef struct Rec {
    Bits bits;
    Kind kind;
    Ref refs[3];
    void (*update)(struct Rec *);
    s16 grid[2][3];
    struct { u8 x; u8 y; } pos;
} Rec;
Rec *ds_probe_0;
"""


def fields(rec):
    return {f["name"]: f for f in rec["fields"]}


have_cc = (ROOT / configure.CC).exists()
if not have_cc:
    print("fixture and real headers: skipped (no compiler; run make setup)")
else:
    with tempfile.TemporaryDirectory() as tmp:
        src, obj = Path(tmp) / "fixture.c", Path(tmp) / "fixture.o"
        src.write_text(FIXTURE)
        text, err = dsindex.compile_probe(src, obj)
    check("fixture compiles", not err, err)
    types = {t["name"]: t for t in dsindex.dwarf_types(text)}
    bits = fields(types["Bits"])
    check("bit fields", [bits[n]["bits"] for n in "abc"] == [[0, 3], [3, 5], [8, 8]],
          [bits[n].get("bits") for n in "abc"])
    check("bit field signedness", bits["c"]["signed"] is True and bits["a"]["signed"] is False)
    ref = types["Ref"]
    check("unnamed member lifted", [f["name"] for f in ref["fields"]] == ["index", "group", "id"],
          [f["name"] for f in ref["fields"]])
    check("enum values", types["Kind"]["values"] == [["KIND_A", 0], ["KIND_B", 5]], types["Kind"])
    rec = fields(types["Rec"])
    check("enum is one byte", rec["kind"]["size"] == 1 and rec["refs"]["offset"] == 4, rec["kind"])
    check("array of unions", rec["refs"]["count"] == 3 and rec["refs"]["struct"] == "Ref"
          and rec["refs"]["size"] == 12, rec["refs"])
    check("function pointer", rec["update"]["type"] == "void (*)(Rec *)", rec["update"]["type"])
    check("2-d array", rec["grid"]["type"] == "s16[2][3]" and rec["grid"]["count"] == 6
          and rec["grid"]["elem_signed"] is True, rec["grid"])
    check("anonymous struct member", [m["name"] for m in rec["pos"].get("members", [])] == ["x", "y"]
          and rec["pos"]["offset"] == 0x20, rec["pos"])
    check("struct size", types["Rec"]["size"] == 0x24, types["Rec"]["size"])

    # Real headers: renames jobs retype and rename fields, so check only the
    # sizes the headers assert themselves (`sizeof(X) == N`).
    SIZE_CHECK = re.compile(r"sizeof\((\w+)\)\s*==\s*(0x[0-9a-fA-F]+|\d+)")
    asserted, wrong = 0, []
    for h in sorted((ROOT / "include").rglob("*.h")):
        want = {n: int(v, 0) for n, v in SIZE_CHECK.findall(h.read_text(errors="replace"))}
        if not want:
            continue
        rec = dsindex.probe_header(h, "test-" + dsindex.cache_key(h))
        got = {st["name"]: st["size"] for st in rec["structs"]}
        for n, v in want.items():
            asserted += 1
            if got.get(n) != v:
                wrong.append(f"{n}: header says {v:#x}, DWARF {got.get(n)}")
    check(f"real headers: {asserted} asserted struct sizes", asserted > 0 and not wrong, wrong)


import ds  # noqa: E402

LINES = [
    (("ldrsh", "r0, [r4, #100]"), ("ldr", 2, True, "r4", 100)),
    (("ldrh", "r1, [r0, #0x64]"), ("ldr", 2, False, "r0", 0x64)),
    (("strb", "r2, [r7, #11]"), ("str", 1, False, "r7", 11)),
    (("ldrsbne", "r2, [r7]"), ("ldr", 1, True, "r7", 0)),
    (("ldr", "r3, [r5, #-4]!"), ("ldr", 4, False, "r5", -4)),
    (("ldr", "r3, [r5], #4"), ("ldr", 4, False, "r5", 0)),
    (("ldr", "r0, [pc, #12]"), None),
    (("ldrb", "r0, [r1, r2]"), None),
    (("ldmia", "sp!, {r4, pc}"), None),
]
for (mn, ops), want in LINES:
    got = ds.mem_access(mn, ops)
    check(f"parse {mn} {ops}", got == want, got)

# dupes on a made-up index: two headers with one layout, a local copy, a
# union whose two views overlap, and a symbol declared two ways.
def fld(name, off, size, **kw):
    return dict(name=name, offset=off, size=size, type="u16" if size == 2 else "u8", signed=False, **kw)


A = {"name": "LevelA", "kind": "struct", "size": 4, "fields": [fld("lv", 0, 1), fld("up", 1, 1), fld("pts", 2, 2)]}
B = {"name": "RankB", "kind": "struct", "size": 4, "fields": [fld("rank", 0, 1), fld("frame", 1, 1), fld("exp", 2, 2)]}
BIG = {"name": "BigTask", "kind": "struct", "size": 0x70, "fields": [fld("pad_00", 0, 0x66), fld("x", 0x66, 2),
                                                                   fld("y", 0x68, 2)]}
V1 = {"name": "ViewOne", "kind": "struct", "size": 4, "fields": [fld("a", 0, 2), fld("b", 2, 2)]}
V2 = {"name": "ViewTwo", "kind": "struct", "size": 6, "fields": [fld("a", 0, 2), fld("b", 2, 2), fld("c", 4, 2)]}
U = {"name": "Holder", "kind": "struct", "size": 8, "fields": [
    fld("one", 2, 4, struct="ViewOne"), fld("two", 2, 6, struct="ViewTwo")]}
LOCAL = {"name": "Unk_local", "kind": "struct", "size": 0x6a, "fields": [fld("pad_00", 0, 0x66), fld("unk_66", 0x66, 2),
                                                                         fld("unk_68", 0x68, 2)]}
fake = {"headers": {"include/t.h": {"key": "", "error": "", "structs": [A, B, BIG, V1, V2, U]}},
        "sources": {"src/t.c": {"key": "", "error": "", "structs": [LOCAL]}},
        "externs": {"data_1": [{"type": "LevelA", "count": None, "file": "include/t.h", "line": 1},
                               {"type": "RankB *", "count": None, "file": "src/t.c", "line": 2}]}}
real_index, real_includers = ds.index, ds.includers
ds.index = lambda: fake
ds.includers = lambda header: frozenset()
check("dupes: layouts", any("LevelA" in r and "RankB" in r for r in ds.dupe_layouts()), ds.dupe_layouts())
check("dupes: locals", any(r.startswith("Unk_local") and "BigTask" in r for r in ds.dupe_locals()), ds.dupe_locals())
check("dupes: unions", ds.dupe_unions() == ["Holder.one lies within Holder.two at 0x2 (include/t.h:0)"],
      ds.dupe_unions())
check("dupes: externs", len(ds.dupe_externs()) == 1 and ds.dupe_externs()[0].startswith("data_1:"), ds.dupe_externs())
ds.index, ds.includers = real_index, real_includers

if not (ROOT / "extract/arm9/arm9.bin").exists():
    print("original code: skipped (no extract/; run make)")
elif have_cc:
    # By address: the original bytes never change, names do.
    f = next((g for gs in ds.fn_index().by_name.values() for g in gs
              if g["module"] == "main" and g["addr"] == 0x02095f28), None)
    acc = [mn for _, mn, _, _ in ds.accesses(f, 0xb, 0xc)] if f else []
    check("main 0x02095f28 reads offset 0xb with ldrb", "ldrb" in acc, acc)
    table = next(n for a, n, k in ds.symbol_table()["main"] if a == 0x0215360c)
    inside = ds.containing("main", 0x02153634)
    check("main 0x02153634 inside the CO table", any(c.startswith(f"{table}+0x28 ") for c in inside), inside)

sys.exit(1 if failures else 0)
