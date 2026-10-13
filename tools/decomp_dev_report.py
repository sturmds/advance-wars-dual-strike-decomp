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
linked when its file is `complete`. This keeps that only for the functions
in config/AWRE/asm_functions.txt: hand-written code that matches the ROM,
with portable C (or an `#error`) in the `#else` branch of the
`#ifdef __MWERKS__` around it, and a reason taken from the ROM's
instructions. Every other `asm` function is taken out of `matched_code`,
`matched_functions` and `complete_code`, in each unit and in the totals.
The report stops with an error when a listed function is not `asm`, when
its file has no portable side for it, or when an `asm` function with a
portable side is missing from the list. A listed function counts as
decompiled only where objdiff scores it 100%, after the rule below.
PROGRESS.md's own columns count
`asm` as neither. objdiff's `total_code` also holds code in gap units that
dsd gives no function, which progress.py does not count.

In a `complete` unit, a function objdiff scores below 100% counts as
matched when every differing row is one `fn.py diff` marks `=`: a
relocation to the same address under another name, Isd_VFormat's last
literal pool word (issue 0074), a branch to the same address, a plain word
equal to our relocated one, or the same bytes decoded differently
(issue 0077: branches into neighbouring functions, data words in code).

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
import re
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
# Functions written as `asm` that count as decompiled (see the docstring).
ASM_LIST = ROOT / "config/AWRE/asm_functions.txt"
# Gap units of the ARM7 project's main, ITCM and DTCM modules. Those modules
# are the made-up ARM9 program of tools/arm7.py's stand-in ROM, not ROM code:
# the linked copy is thrown away, so the report leaves them out.
STAND_IN_UNIT = re.compile(r"_dsd_gap@(main|itcm|dtcm)_\d+")


def merged_units() -> list[dict]:
    units = []
    for cpu, project in CPUS.items():
        base = ROOT / project
        for unit in json.loads((base / "objdiff.json").read_text())["units"]:
            if cpu == "arm7" and STAND_IN_UNIT.fullmatch(unit["name"]):
                continue
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


def read_asm_list() -> dict[tuple[str, str], str]:
    """{(module, name): reason} from ASM_LIST."""
    out = {}
    for n, line in enumerate(ASM_LIST.read_text().splitlines(), 1):
        if not line.strip() or line.lstrip().startswith("#"):
            continue
        parts = line.split(None, 2)
        if len(parts) < 3:
            sys.exit(f"decomp_dev_report: {ASM_LIST.relative_to(ROOT)}:{n}: want a module, a name and a reason")
        if (parts[0], parts[1]) in out:
            sys.exit(f"decomp_dev_report: {ASM_LIST.relative_to(ROOT)}:{n}: {parts[1]} is listed twice")
        out[(parts[0], parts[1])] = parts[2]
    return out


def asm_functions(text: str) -> dict[str, bool]:
    """{name: portable} for each function a source file writes as `asm`.
    Portable means every `asm` line of the name sits in the first branch of
    an `#ifdef __MWERKS__` whose `#else` branch is not empty (C, or an
    `#error`)."""
    lines = text.splitlines()
    stack = []  # [mwerks, in_else, else_has_text] per open conditional
    where = {}  # line -> (innermost frame, in its first branch)
    for i, line in enumerate(lines):
        word = line.strip()
        if word.startswith("#if"):
            stack.append([word.split(None, 1)[-1] == "__MWERKS__" and word.startswith("#ifdef"), False, False])
        elif word.startswith("#el") and stack:
            stack[-1][1] = True
        elif word.startswith("#endif") and stack:
            stack.pop()
        elif stack:
            if stack[-1][1] and word:
                stack[-1][2] = True
            where[i] = (stack[-1], not stack[-1][1])
    out = {}
    for m in progress.ASM_FUNC.finditer(text):
        frame, first = where.get(text.count("\n", 0, m.start(1)), (None, False))
        portable = bool(frame and first and frame[0] and frame[2])
        out[m.group(1)] = out.get(m.group(1), True) and portable
    return out


def source_modules() -> dict[str, str]:
    """{source path: module} over every module's delinks.txt."""
    return {p: mod.name for mod in fn.all_modules().values() for p, *_ in mod.files}


def check_asm_list(listed: dict[tuple[str, str], str], modules: dict[str, str]) -> None:
    """Exit when ASM_LIST and the source disagree."""
    found = {}  # (module, name) -> (path, portable)
    for path, module in modules.items():
        if (ROOT / path).exists():
            for name, portable in asm_functions((ROOT / path).read_text()).items():
                found[(module, name)] = (path, portable)
    errors = []
    for key in listed:
        if key not in found:
            errors.append(f"{key[1]} ({key[0]}) is listed but no source file of {key[0]} writes it as asm")
        elif not found[key][1]:
            errors.append(f"{key[1]} ({key[0]}) is listed but {found[key][0]} has no portable side for it")
    for key, (path, portable) in sorted(found.items()):
        if portable and key not in listed:
            errors.append(f"{key[1]} ({key[0]}) is asm with a portable side in {path} but is not listed")
    if errors:
        sys.exit(f"decomp_dev_report: {ASM_LIST.relative_to(ROOT)}:\n  " + "\n  ".join(errors))


def add(target: dict, key: str, amount: int) -> None:
    """Add to a measure. objdiff writes byte counts as strings."""
    if key.endswith("_code"):
        target[key] = str(int(target.get(key, 0)) + amount)
    else:
        target[key] = target.get(key, 0) + amount


def count_same_rows(report: dict, units: list[dict], modules: dict[str, str]) -> None:
    """Count as matched a function of a `complete` unit whose only differing
    rows `fn.py diff` marks `=` (issues 0074 and 0077, fn.score_rows), with
    both objects given so that rows can match by their bytes. Real
    differences still count."""
    paths = {u["name"]: u for u in units}
    totals = report["measures"]
    for unit in report["units"]:
        u = paths.get(unit["name"], {})
        if not u.get("metadata", {}).get("complete") or "base_path" not in u:
            continue
        module = modules.get(u["metadata"].get("source_path"))
        for f in unit.get("functions", []):
            if f.get("fuzzy_match_percent") in (None, 100.0):
                continue
            objects = (PROJECT / u["target_path"], PROJECT / u["base_path"])
            data = fn.objdiff(*objects, f["name"])
            scored = data and fn.score_rows(data, f["name"], module, objects)
            if not scored:
                continue
            left, right, same, _ = scored
            if same and fn.count_differ(left, right, same) == 0:
                f["fuzzy_match_percent"] = 100.0
                for target in (unit["measures"], totals):
                    add(target, "matched_code", int(f["size"]))
                    add(target, "matched_functions", 1)


def drop_asm(report: dict, units: list[dict], modules: dict[str, str],
             listed: dict[tuple[str, str], str]) -> None:
    """Count functions written as `asm` as neither matched nor linked,
    unless ASM_LIST has them."""
    sources = {u["name"]: u["metadata"].get("source_path") for u in units}
    totals = report["measures"]
    for unit in report["units"]:
        source = sources.get(unit["name"])
        if not source or not (ROOT / source).exists():
            continue
        module = modules.get(source)
        asm = {n for n in progress.ASM_FUNC.findall((ROOT / source).read_text()) if (module, n) not in listed}
        m = unit["measures"]
        linked = int(m.get("complete_code", 0)) > 0
        for f in unit.get("functions", []):
            if f["name"] not in asm:
                continue
            matched = f.get("fuzzy_match_percent") == 100.0
            for target in (m, totals):
                if matched:
                    add(target, "matched_code", -int(f["size"]))
                    add(target, "matched_functions", -1)
                if linked:
                    add(target, "complete_code", -int(f["size"]))
    for m in [totals] + [u["measures"] for u in report["units"]]:
        code = int(m.get("total_code", 0))
        m["matched_code_percent"] = percent(int(m.get("matched_code", 0)), code)
        m["complete_code_percent"] = percent(int(m.get("complete_code", 0)), code)
        m["matched_functions_percent"] = percent(m.get("matched_functions", 0), m.get("total_functions", 0))


def main() -> int:
    modules = source_modules()
    listed = read_asm_list()
    check_asm_list(listed, modules)
    subprocess.run(fn.ninja("objdiff.json", "build/arm7/objdiff.json"), cwd=ROOT, check=True, capture_output=True)
    units = merged_units()
    PROJECT.mkdir(parents=True, exist_ok=True)
    config = {"min_version": "2.3.2", "build_base": False, "build_target": False, "units": units}
    (PROJECT / "objdiff.json").write_text(json.dumps(config, indent=1) + "\n")
    report = PROJECT / "report.json"
    subprocess.run([fn.OBJDIFF, "report", "generate", "-p", PROJECT, "-o", report],
                   cwd=ROOT, check=True, capture_output=True)
    data = json.loads(report.read_text())
    count_same_rows(data, units, modules)
    drop_asm(data, units, modules, listed)
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
