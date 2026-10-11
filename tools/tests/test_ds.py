#!/usr/bin/env python3
"""Check tools/dsindex.py and tools/ds.py (issue 0041).

    python3 tools/tests/test_ds.py

1. A fixture header compiled with the pinned mwccarm: bit-field positions,
   unions, arrays, an unnamed struct member, an enum and a function pointer.
2. The load and store parser on fixed objdump lines.
3. Real headers: Task is 0x6c bytes with unk_64 an s16, AiSite.score is an
   s8 at 0xb.
4. With extract/ present: AiSite_PickPiperunnerSite reads AiSite.score
   with ldrb, and data_02153634 lies inside gCoTable. Skipped without a ROM.

Needs the compiler from `make setup`; parts 1 and 3 are skipped without it.
"""

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

    def real(name, header):
        h = ROOT / "include" / header
        rec = dsindex.probe_header(h, "test-" + dsindex.cache_key(h))
        return next((s for s in rec["structs"] if s["name"] == name), None), rec["error"]

    task, err = real("Task", "game/task.h")
    check("Task layout", task and task["size"] == 0x6c and fields(task)["unk_64"]["type"] == "s16"
          and fields(task)["unk_64"]["offset"] == 0x64, err or task and task["size"])
    site, err = real("AiSite", "game/ai_site.h")
    score = site and fields(site)["score"]
    check("AiSite.score", score and score["type"] == "s8" and score["offset"] == 0xb, err or score)

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

if not (ROOT / "extract/arm9/arm9.bin").exists():
    print("original code: skipped (no extract/; run make)")
elif have_cc:
    f = ds.original("AiSite_PickPiperunnerSite", ROOT / "src/game/main/ai/ai_site_pick_piperunner_site.c")
    acc = [mn for _, mn, _, _ in ds.accesses(f, 0xb, 0xc)] if f else []
    check("AiSite.score read with ldrb", "ldrb" in acc, acc)
    inside = ds.containing("main", 0x02153634)
    check("data_02153634 inside gCoTable", any(c.startswith("gCoTable+0x28") for c in inside), inside)

sys.exit(1 if failures else 0)
