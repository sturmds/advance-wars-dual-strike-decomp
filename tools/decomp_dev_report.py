#!/usr/bin/env python3
"""Progress report in the form decomp.dev reads.

    decomp_dev_report.py

Writes build/artifacts/0.1_report/report.json: one objdiff report over ARM9
and ARM7. decomp.dev takes reports from GitHub Actions artifacts named
`<version>_report` and shows each report's `matched_code_percent` as
decompiled and `complete_code_percent` as fully linked. The directory under
build/artifacts/ holds what such an artifact would hold. Nothing here uploads
it.

objdiff-cli has no command to merge two reports, so this writes one objdiff
project with the units of both CPUs (build/report/objdiff.json) and lets
objdiff-cli measure it. ARM7 units outside src/arm7/ get an `arm7/` prefix:
both CPUs have gap units such as `_dsd_gap@main_1`.

objdiff counts a function written as `asm` in a .c file as matched, and as
linked when its file is `complete`. PROGRESS.md does not, so this takes those
functions out of `matched_code`, `matched_functions` and `complete_code`, in
each unit and in the totals. The linked bytes then equal PROGRESS.md's "Bytes
in C". The percentage can still differ a little: objdiff's `total_code` also
holds code in gap units that dsd gives no function, which progress.py does
not count.

The report is read back
with `objdiff-cli report changes` to check that it parses. It is not
written if it matches a private pattern (tenet 8) or has a library-style
name with no public source (tenet 10), the checks of tools/pnc_check.py.

`make release` runs this after `make check`. The artifact name and the two
measures come from decomp.dev's source (docs/third-party/decomp-dev.md).
Uploading it is issue 0071.
"""

import json
import os
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import fn  # noqa: E402
import pnc_check  # noqa: E402
import progress  # noqa: E402

ROOT = fn.ROOT
# decomp.dev's version label, taken from the artifact name. It is 0.1 for now;
# a real label (the game code) waits for the rest of the onboarding.
VERSION = "0.1"
PROJECT = ROOT / "build/report"
ARTIFACT = ROOT / "build/artifacts" / f"{VERSION}_report"
# objdiff project directory and objdiff.json of each CPU, relative to ROOT.
CPUS = {"arm9": ".", "arm7": "build/arm7"}


def merged_units() -> list[dict]:
    units = []
    for cpu, project in CPUS.items():
        base = ROOT / project
        for unit in json.loads((base / "objdiff.json").read_text())["units"]:
            unit = dict(unit, metadata=dict(unit.get("metadata", {})))
            for key in ("target_path", "base_path"):
                if key in unit:
                    unit[key] = os.path.relpath(base / unit[key], PROJECT)
            if "source_path" in unit["metadata"]:
                path = os.path.normpath(base / unit["metadata"]["source_path"])
                unit["metadata"]["source_path"] = os.path.relpath(path, ROOT)
            if cpu == "arm7" and not unit["name"].startswith("src/arm7/"):
                unit["name"] = f"arm7/{unit['name']}"
            units.append(unit)
    names = [u["name"] for u in units]
    if len(set(names)) != len(names):
        sys.exit("decomp_dev_report: two units have the same name")
    return units


def percent(part: int, whole: int) -> float:
    return 100 * part / whole if whole else 0.0


def drop_asm(report: dict, units: list[dict]) -> None:
    """Count functions written as `asm` as neither matched nor linked."""
    sources = {u["name"]: u["metadata"].get("source_path") for u in units}
    totals = report["measures"]
    for unit in report["units"]:
        source = sources.get(unit["name"])
        if not source or not (ROOT / source).exists():
            continue
        asm = set(progress.ASM_FUNC.findall((ROOT / source).read_text()))
        m = unit["measures"]
        linked = int(m.get("complete_code", 0)) > 0
        for f in unit.get("functions", []):
            if f["name"] not in asm:
                continue
            matched = f.get("fuzzy_match_percent") == 100.0
            for target in (m, totals):
                if matched:
                    target["matched_code"] = str(int(target.get("matched_code", 0)) - int(f["size"]))
                    target["matched_functions"] = target.get("matched_functions", 0) - 1
                if linked:
                    target["complete_code"] = str(int(target.get("complete_code", 0)) - int(f["size"]))
    for m in [totals] + [u["measures"] for u in report["units"]]:
        code = int(m.get("total_code", 0))
        m["matched_code_percent"] = percent(int(m.get("matched_code", 0)), code)
        m["complete_code_percent"] = percent(int(m.get("complete_code", 0)), code)
        m["matched_functions_percent"] = percent(m.get("matched_functions", 0), m.get("total_functions", 0))


def main() -> int:
    subprocess.run(fn.ninja("objdiff.json", "build/arm7/objdiff.json"), cwd=ROOT, check=True, capture_output=True)
    units = merged_units()
    PROJECT.mkdir(parents=True, exist_ok=True)
    config = {"min_version": "2.3.2", "build_base": False, "build_target": False, "units": units}
    (PROJECT / "objdiff.json").write_text(json.dumps(config, indent=1) + "\n")
    report = PROJECT / "report.json"
    subprocess.run([fn.OBJDIFF, "report", "generate", "-p", PROJECT, "-o", report],
                   cwd=ROOT, check=True, capture_output=True)
    data = json.loads(report.read_text())
    drop_asm(data, units)
    report.write_text(json.dumps(data))
    # `report changes` parses both reports; a file it cannot read fails here.
    subprocess.run([fn.OBJDIFF, "report", "changes", "-o", os.devnull, report, report],
                   cwd=ROOT, check=True, capture_output=True)

    data = report.read_bytes()
    pattern = pnc_check.patterns()
    if pattern and (m := pattern.search(data)):
        sys.exit(f"decomp_dev_report: the report matches a private pattern ({m.group(0).decode(errors='replace')!r})")
    if names := pnc_check.unsourced(data, pnc_check.allowed_names()):
        sys.exit("decomp_dev_report: library-style names with no public source (tenet 10): "
                 + ", ".join(sorted(n.decode() for n in names)))
    measures = json.loads(data)["measures"]
    if measures.get("total_units") != len(units):
        sys.exit(f"decomp_dev_report: the report has {measures.get('total_units')} units, the project {len(units)}")

    ARTIFACT.mkdir(parents=True, exist_ok=True)
    (ARTIFACT / "report.json").write_bytes(data)
    print(f"decomp.dev: {(ARTIFACT / 'report.json').relative_to(ROOT)}, "
          f"decompiled {measures.get('matched_code_percent', 0):.2f}%, "
          f"linked {measures.get('complete_code_percent', 0):.2f}%")
    return 0


if __name__ == "__main__":
    sys.exit(main())
