#!/usr/bin/env python3
"""Progress report in the form decomp.dev reads.

    decomp_dev_report.py

Writes build/artifacts/AWRE_report/report.json: one objdiff report over ARM9
and ARM7. decomp.dev takes reports from GitHub Actions artifacts named
`<version>_report` and shows each report's `matched_code_percent` as
decompiled and `complete_code_percent` as fully linked. The directory under
build/artifacts/ holds what such an artifact would hold. hooks/pre-commit
commits a copy as PROGRESS.json, and .github/workflows/decomp-dev.yml
uploads it.

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
objdiff's `total_code` also holds code in gap units that dsd gives no
function. tools/progress.py writes PROGRESS.md from this report.

In a `complete` unit, a function objdiff scores below 100% counts as
matched when every differing row is one `fn.py diff` marks `=`: a
relocation to the same address under another name, Isd_VFormat's last
literal pool word (issue 0074), a branch to the same address, a plain word
equal to our relocated one, or the same bytes decoded differently
(issue 0077: branches into neighbouring functions, data words in code).

It also stops when config/AWRE/aliases.txt and symbols.txt disagree: every
.bss alias must be listed once, with its base and offset (bss.py
alias_list_errors, docs/bss-tool.md "Aliases", issue 0083), and when an
alias's FIELD and the C type of its base give different offsets
(tools/alias_check.py), and when an extern of a data object has another
type than its definition and config/AWRE/extern_types.txt does not list
it (tools/extern_types.py --check), and when a C file copies a header's
declaration or two headers declare one name (tools/decls.py check), and when a
variable of a generated .bss file has a size in its object other than the
one in symbols.txt (bss.py gen_size_errors).

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
import alias_check  # noqa: E402
import decls  # noqa: E402
import extern_types  # noqa: E402
import bss  # noqa: E402
import fn  # noqa: E402
import pnc_check  # noqa: E402
import progress  # noqa: E402

ROOT = fn.ROOT
# decomp.dev's version label, taken from the artifact name: the game code, as
# other DS projects use it (BK9E_report in kh-recoded-decomp).
VERSION = "AWRE"
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
                fuzzy_from_functions(unit)
                raised = True
    if raised:
        # objdiff's total: the units' fuzzy scores weighted by their code size.
        scores = [(u["measures"].get("fuzzy_match_percent", 100.0), int(u["measures"].get("total_code", 0)))
                  for u in report["units"]]
        code = sum(size for _, size in scores)
        if all(score == 100.0 for score, size in scores if size):
            totals["fuzzy_match_percent"] = 100.0
        elif code:
            totals["fuzzy_match_percent"] = sum(score * size for score, size in scores) / code


def fuzzy_from_functions(unit: dict) -> None:
    """A unit's fuzzy score as objdiff computes it: its functions' scores
    weighted by size. decomp.dev draws a unit green in its overview only
    when this is exactly 100, so all functions at 100 give exactly 100.0
    (and the same for the .text section)."""
    functions = unit.get("functions", [])
    size = sum(int(f["size"]) for f in functions)
    if not size:
        return
    if all(f.get("fuzzy_match_percent") == 100.0 for f in functions):
        score = 100.0
    else:
        score = sum(f.get("fuzzy_match_percent", 0.0) * int(f["size"]) for f in functions) / size
    unit["measures"]["fuzzy_match_percent"] = score
    for s in unit.get("sections", []):
        if s["name"] == ".text":
            s["fuzzy_match_percent"] = score


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
    alias_errors = bss.alias_list_errors(bss.read_alias_list(), bss.config_aliases())
    if alias_errors:
        sys.exit(f"decomp_dev_report: {bss.ALIAS_LIST}:\n  " + "\n  ".join(alias_errors))
    aliases, alias_errors = alias_check.read_aliases()
    alias_errors += alias_check.check(aliases, alias_check.compute(aliases), bss.linked_aliases())
    if alias_errors:
        sys.exit("decomp_dev_report: aliases whose FIELD and C type disagree (tools/alias_check.py):\n  "
                 + "\n  ".join(alias_errors))
    extern_errors = extern_types.check()
    if extern_errors:
        sys.exit("decomp_dev_report: externs whose type differs from the definition (tools/extern_types.py;"
                 f" list one on purpose in {extern_types.KNOWN}):\n  " + "\n  ".join(extern_errors))
    decl_errors = decls.check_errors()
    if decl_errors:
        sys.exit("decomp_dev_report: declarations copied from a header or in two headers (tools/decls.py,"
                 f" docs/readability.md; list one on purpose in {decls.EXCEPTIONS.name}):\n  "
                 + "\n  ".join(decl_errors))
    size_errors = bss.gen_size_errors()
    if size_errors:
        sys.exit("decomp_dev_report: generated .bss sizes:\n  " + "\n  ".join(size_errors))
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
          f"decompiled {progress.pct(int(measures.get('matched_code', 0)), int(measures.get('total_code', 0)))}, "
          f"linked {progress.pct(int(measures.get('complete_code', 0)), int(measures.get('total_code', 0)))}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
