#!/usr/bin/env python3
"""Check tools/alias_check.py: how it reads FIELD, and the offsets it gets
from mwccarm for a small file.

    python3 tools/tests/test_alias_check.py

No ROM or build needed. The compile part uses the compiler from `make setup`
and skips with a message without it.
"""

import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
import alias_check  # noqa: E402
import configure  # noqa: E402
import shift_gaps  # noqa: E402

errors = []

LIST = """\
# comment
main a1 = gTable+0x4 real [1]
main a2 = gTable+0xa real [2].y (note with spaces)
main a3 = gTable+0x9 real [2].x+0x1
main a4 = gWork+0x1000 real +0x1000 (compiler base)
main a5 = gWork+0xa real items[1].y
main a6 = gWork+0x4 real items
main a7 = gTable+0x4 real [0].z
main a8 = gTable+0x8 real [1]
main a9 = gTable+0x0 unsorted
main b1 = gWork+0x10 base (inside Work)
main b2 = gWork+0x100 base (past its end)
"""

C = """\
typedef struct Pair {
  unsigned short x;
  unsigned short y;
} Pair;
typedef struct Work {
  int count;
  Pair items[4];
} Work;
Pair gTable[8];
static Work gWork;
void Use(void) { gWork.count = 1; }
"""

with tempfile.TemporaryDirectory(prefix="test_alias_check") as d:
    path = Path(d) / "aliases.txt"
    path.write_text(LIST)
    aliases, read_errors = alias_check.read_aliases(path)
    by = {a.name: a for a in aliases}
    if read_errors:
        errors.append(f"read: {read_errors}")
    want = {"a1": ("[1]", 0), "a2": ("[2].y", 0), "a3": ("[2].x", 1), "a4": ("", 0x1000),
            "a5": (".items[1].y", 0), "a6": (".items", 0)}
    for name, (p, extra) in want.items():
        if (by[name].path, by[name].extra) != (p, extra):
            errors.append(f"read {name}: got {by[name].path!r}+{by[name].extra}, want {p!r}+{extra}")
    if by["a9"].field is not None:
        errors.append("read a9: an unsorted alias has no FIELD")
    if not by["b1"].base_group or by["a4"].base_group or by["b1"].field is not None:
        errors.append("read b1: want the base group and no FIELD")
    not_c = sorted(a.name for a in alias_check.not_from_c(aliases))
    if not_c != ["a3", "a4", "a9"]:
        errors.append(f"not_from_c: got {not_c}")
    # A base alias is checked another way and is no shift gap.
    sites = [n for _, _, n, _ in shift_gaps.alias_sites(path)]
    if sites != [4, 5, 10]:
        errors.append(f"shift_gaps.alias_sites: got lines {sites}, want [4, 5, 10]")

    config = Path(d) / "config"
    (config / "arm9").mkdir(parents=True)
    (config / "arm9/symbols.txt").write_text(
        "b1 kind:bss(size=0x0) addr:0x02000010\n"
        "b2 kind:bss(size=0x0) addr:0x02000100\n"
        "Use kind:function(arm,size=0x20) addr:0x02100000\n")
    # b1 sits in Use's literal pool; b2 only in data, which no compiler base is.
    (config / "arm9/relocs.txt").write_text(
        "from:0x0210001c kind:load to:0x02000010 module:main\n"
        "from:0x02200000 kind:load to:0x02000100 module:main\n"
        "from:0x02100004 kind:load to:0x02000100 module:overlay(1)\n")
    saved, alias_check.CONFIG = alias_check.CONFIG, config
    try:
        loads = alias_check.code_loads([by["b1"], by["b2"]])
    finally:
        alias_check.CONFIG = saved
    if loads != {"b1": 1, "b2": 0}:
        errors.append(f"code_loads: got {loads}, want b1 1 and b2 0")

    if not (configure.ROOT / configure.CC).exists():
        print("test_alias_check: no mwccarm in .cache (run `make setup`); compile part skipped")
    else:
        src = Path(d) / "work.c"
        src.write_text(C)
        results = alias_check.check_file(str(src), [a for a in aliases if a.field is not None or a.base_group])
        if results.get("b1", {}).get("size") != 0x14:
            errors.append(f"check_file: want the size of Work (0x14) for b1, got {results.get('b1')}")
        for name, count in loads.items():
            results[name]["loads"] = count
        got = alias_check.check(aliases, results)
        # a7 names a field Pair does not have; a8 sits on [2], not [1]; b2 lies past the end of
        # gWork and no function loads it.
        if (len(got) != 4 or "a7" not in got[0] or "a8" not in got[1] or "+0x4" not in got[1]
                or "b2" not in got[2] or "not inside" not in got[2] or "no function" not in got[3]):
            errors.append("check: want errors for a7, a8 and b2 (twice) only, got\n    " + "\n    ".join(got))
        if results.get("a4", {}).get("offset") != 0:
            errors.append(f"check: a FIELD of only a byte count gives offset 0 from the path, got {results.get('a4')}")

if errors:
    sys.exit("test_alias_check:\n  " + "\n  ".join(errors))
print("test_alias_check: ok")
