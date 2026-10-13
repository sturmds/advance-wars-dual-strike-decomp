#!/usr/bin/env python3
"""Check the PROGRESS.md that progress.py writes from a small report.

    python3 tools/tests/test_progress.py

The orchestrator reads the `all` row, so its format is checked here. Uses
the repo's config/ for module names; no ROM or build needed.
"""

import sys
from pathlib import Path

TOOLS = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(TOOLS))

import progress  # noqa: E402


def unit(name, source, code, matched, funcs, total_funcs, functions=()):
    meta = {"source_path": source} if source else {}
    measures = {"total_code": str(code), "matched_code": str(matched), "complete_code": str(matched),
                "matched_functions": funcs, "total_functions": total_funcs}
    return {"name": name, "metadata": meta, "measures": measures, "functions": list(functions)}


report = {
    "measures": {"total_code": "300", "matched_code": "200", "complete_code": "150",
                 "matched_functions": 3, "total_functions": 4},
    "units": [
        unit("src/game/a", "src/game/a.c", 100, 100, 2, 2),
        unit("src/sdk/b", "src/sdk/b.c", 100, 100, 1, 1),
        unit("arm7/_dsd_gap@autoload_3_0", None, 100, 0, 0, 1,
             [{"name": "data_arm7_03807180", "size": "100"}]),
    ],
}
text = progress.markdown(report, "build/artifacts/AWRE_report/report.json")
rows = [line for line in text.splitlines() if line.startswith("| all |")]
want = "| all | 66.6666% | 50.0000% | 200 / 300 | 150 / 300 | 3 / 4 |"
errors = []
if rows != [want]:
    errors.append(f"all row: want [{want!r}], got {rows!r}")
for line in ("| arm7 | 0.0000% | 0.0000% | 0 / 100 | 0 / 100 | 0 / 1 |",
             "| game | 100.0000% | 100.0000% | 100 / 100 | 100 / 100 | 2 / 2 |",
             "| arm7 | `arm7/_dsd_gap@autoload_3_0` | data_arm7_03807180 | 100 | 0.00% |"):
    if line not in text.splitlines():
        errors.append(f"missing line {line!r}")
for part, whole, want in ((1844958, 1845022, "99.9965%"), (999999, 1000000, "99.9999%"),
                          (1844958, 1844958, "100.0000%"), (0, 0, "-"), (2, 3, "66.6666%")):
    if progress.pct(part, whole) != want:
        errors.append(f"pct({part}, {whole}): want {want}, got {progress.pct(part, whole)}")
if errors:
    sys.exit("test_progress: " + "\n  ".join(errors))
print("test_progress: ok")
