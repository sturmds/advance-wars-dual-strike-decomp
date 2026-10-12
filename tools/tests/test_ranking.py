#!/usr/bin/env python3
"""Checks that partials rank by differing instructions first (issue 0067).

    python3 tools/tests/test_ranking.py

Reads a sample of `fn.py diff` output with fn.parse_diff, and sorts the
three CampaignMap_Init versions from the issue with fn.rank,
triage_partials.sort_key and propagate.better. Needs no build and no ROM.
"""

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
import fn  # noqa: E402
import propagate  # noqa: E402
import triage_partials  # noqa: E402

# fn.py diff --color never: one = row (issue 0017), one opcode that differs,
# one row only the original has.
ROWS = [("   0: push {r4, lr}", "   0: push {r4, r5, lr}", "|"),
        ("   4: ldr r0, [pc, #0x8]", "   4: ldr r0, [pc, #0x8]", "="),
        ("   8: bl 0x0", "   8: bl 0x0", " "),
        ("   c: pop {r4, pc}", "", "|")]
WIDTH = max(len(a) for a, _, _ in ROWS) + 2
SAMPLE = "\n".join([f"{'original':<{WIDTH}}   ours",
                    *(f"{a:<{WIDTH}} {m} {b}" for a, b, m in ROWS),
                    "",
                    "= : 1 relocation(s) name another symbol at the same address, or an absolute symbol"
                    " of the same value; counted as matching.",
                    "",
                    "Sample: 2 differ, 87.5% match", ""])

# (differ, objdiff percent) for the three versions in the issue's table.
SEVEN_REGS = (7, 99.9275)
ONE_OPCODE = (1, 99.9078)
TWO_OPCODES = (2, 99.8287)


def test_parse():
    s = fn.parse_diff(SAMPLE)
    assert s is not None
    assert (s.differ, s.percent) == (2, 87.5), (s.differ, s.percent)
    assert s.rows == [(a.strip(), b.strip(), m) for a, b, m in ROWS], s.rows
    # The count printed agrees with the rows marked |.
    assert s.differ == sum(r[2] == "|" for r in s.rows)
    assert fn.parse_diff(SAMPLE.replace("87.5%", "?%")).percent is None
    assert fn.parse_diff("error: build failed\n") is None
    # The old last line, without a count, is not read as a score.
    assert fn.parse_diff(SAMPLE.replace("2 differ, ", "")) is None


def test_rank():
    order = sorted([SEVEN_REGS, ONE_OPCODE, TWO_OPCODES], key=lambda v: fn.rank(*v))
    assert order == [ONE_OPCODE, TWO_OPCODES, SEVEN_REGS], order
    # Same count: the higher percentage first.
    assert fn.rank(3, 99.5) < fn.rank(3, 99.0)
    assert fn.rank(0, None) < fn.rank(1, 100.0)


def test_triage_sort():
    rows = [{"name": "seven", "class": "structural", "differ": 7, "pct": 99.9275},
            {"name": "one", "class": "structural", "differ": 1, "pct": 99.9078},
            {"name": "two", "class": "structural", "differ": 2, "pct": 99.8287},
            {"name": "nodiff", "class": "structural", "pct": None},
            {"name": "regs", "class": "register-only", "differ": 40, "pct": 90.0}]
    for r in rows:
        r.update(module="main", size=0x100)
    got = [r["name"] for r in sorted(rows, key=triage_partials.sort_key)]
    assert got == ["regs", "one", "two", "seven", "nodiff"], got
    md = triage_partials.markdown(rows)
    head = next(ln for ln in md.splitlines() if ln.startswith("| Function"))
    assert head.split(" | ")[3:5] == ["Differ rows", "Match"], head
    assert "| one | " in md and "| 1 | 99.9078% |" in md, md


def test_propagate_better():
    def r(v):
        return {"status": "differ", "differ": v[0], "percent": v[1]}
    best = None
    for v in (SEVEN_REGS, ONE_OPCODE, TWO_OPCODES):
        if propagate.better(r(v), best):
            best = r(v)
    assert (best["differ"], best["percent"]) == ONE_OPCODE, best
    assert propagate.better(r(SEVEN_REGS), {"status": "compile-error"})
    assert not propagate.better({"status": "compile-error"}, r(SEVEN_REGS))


def main() -> int:
    tests = [test_parse, test_rank, test_triage_sort, test_propagate_better]
    for t in tests:
        t()
    print(f"test_ranking: {len(tests)} checks passed")
    return 0


if __name__ == "__main__":
    sys.exit(main())
