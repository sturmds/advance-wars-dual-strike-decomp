#!/usr/bin/env python3
"""Decompilation progress (plan D4), as decomp.dev measures it.

    progress.py                  print the report as Markdown
    progress.py --write FILE     write it to FILE (PROGRESS.md), and copy the
                                 report beside it as FILE.json (PROGRESS.json),
                                 which the decomp.dev workflow uploads

Reads build/artifacts/AWRE_report/report.json, which tools/decomp_dev_report.py
writes in `make release`, and gives decomp.dev's two measures: decompiled
(`matched_code`) and fully linked (`complete_code`), in bytes and as a share
of the code, with the functions matched. One row for all code, then rows by
module (main, itcm, ov000-ov006, arm7) and by kind of code (game, SDK/library,
and dsd gap units, which have no source file). Then every symbol in code
that the report scores below 100%, and every function written as `asm` that
the report leaves out (config/AWRE/asm_functions.txt does not list it).

The row for all code reads, for example:

    | all | 99.9514% | 99.9514% | 1844182 / 1845078 | 1844182 / 1845078 | 8559 / 8572 |

Percentages are cut off at four decimals, never rounded, so 100.0000% shows
only when the counts are equal. The same report gives the same file. Without the report it stops with an
error: run `make release` first, as hooks/pre-commit and land_self.py do.

Other tools import ASM_FUNC (decomp_dev_report.py) and unlinked()
(protocheck.py, triage_partials.py).
"""

import json
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import fn  # noqa: E402

ROOT = fn.ROOT
ASM_FUNC = re.compile(r"^\s*asm\s+[\w\s\*]*?\b(\w+)\s*\(", re.M)
# Kinds of code, by the start of a unit's source path; dsd gap units have none.
AREAS = (("game", ("src/game/",)), ("SDK/library", ("src/sdk/", "src/arm7/")))
GAP = "gap units"
MEASURES = ("total_code", "matched_code", "complete_code", "matched_functions", "total_functions")


def unlinked():
    """[(path, module, functions)] of source files in src/ that are not
    marked complete: started, but not built into the ROM yet. A file with no
    delinks entry has no module and no functions."""
    out, listed = [], set()
    for mod in fn.all_modules().values():
        for path, start, end, complete in mod.files:
            listed.add(path)
            if complete or not path.startswith("src/") or not (ROOT / path).exists():
                continue
            out.append((path, mod.name, [f for f in mod.funcs if start <= f.addr < end]))
    for p in sorted((ROOT / "src").rglob("*.c")):
        path = p.relative_to(ROOT).as_posix()
        if path not in listed:
            out.append((path, None, []))
    return sorted(out, key=lambda x: x[0])


def pct(part: int, whole: int) -> str:
    """Percentage cut off (never rounded) at 4 decimals, by integer
    arithmetic: 100.0000% only when part == whole, else at most 99.9999%."""
    if not whole:
        return "-"
    scaled = 1000000 * part // whole
    return f"{scaled // 10000}.{scaled % 10000:04d}%"


def unit_module(unit: dict, modules: dict[str, str]) -> str:
    """The module of a report unit: by its source file, or for a gap unit
    (`_dsd_gap@ov000_3`) by its name. ARM7 units are `arm7`."""
    name = unit["name"]
    if name.startswith(("arm7/", "src/arm7/")):
        return "arm7"
    source = unit.get("metadata", {}).get("source_path")
    if source in modules:
        return modules[source]
    return name.partition("@")[2].rsplit("_", 1)[0] or "other"


def unit_area(unit: dict) -> str:
    source = unit.get("metadata", {}).get("source_path") or ""
    return next((area for area, starts in AREAS if source.startswith(starts)), GAP)


def measures(m: dict) -> list[int]:
    return [int(m.get(k, 0)) for k in MEASURES]


def row(label: str, values: list[int]) -> str:
    code, matched, linked, funcs, total_funcs = values
    return (f"| {label} | {pct(matched, code)} | {pct(linked, code)} | {matched} / {code} "
            f"| {linked} / {code} | {funcs} / {total_funcs} |")


def header(first: str) -> list[str]:
    return [f"| {first} | Decompiled | Fully linked | Bytes decompiled | Bytes linked | Functions matched |",
            "|---|---:|---:|---:|---:|---:|"]


def markdown(report: dict, report_path: str) -> str:
    import decomp_dev_report

    modules = decomp_dev_report.source_modules()
    order = list(fn.all_modules())
    by_module, by_area = {}, {}
    below = []  # (module, unit, symbol, size, percent)
    for unit in report["units"]:
        mod = unit_module(unit, modules)
        for table, key in ((by_module, mod), (by_area, unit_area(unit))):
            sums = table.setdefault(key, [0] * len(MEASURES))
            for i, v in enumerate(measures(unit["measures"])):
                sums[i] += v
        for f in unit.get("functions", []):
            score = f.get("fuzzy_match_percent") or 0.0
            if score < 100.0:
                below.append((mod, unit["name"], f["name"], int(f["size"]), score))

    # asm functions the report takes out of the counts (decomp_dev_report.drop_asm).
    listed = decomp_dev_report.read_asm_list()
    asm = []
    for path, mod in sorted(modules.items()):
        if (ROOT / path).exists() and path.endswith(".c"):
            asm += [(mod, path, n) for n in ASM_FUNC.findall((ROOT / path).read_text()) if (mod, n) not in listed]

    lines = [
        "# Decompilation progress",
        "",
        "Written by `tools/progress.py --write PROGRESS.md` from `hooks/pre-commit`; do not edit.",
        f"The numbers come from `{report_path}`,",
        "the report `make release` writes for decomp.dev (`tools/decomp_dev_report.py`).",
        "\"Decompiled\" is the code that matches the ROM from C; \"Fully linked\" is the part of it",
        "that the build links from `src/` (files marked `complete` in `delinks.txt`). Functions",
        "written as `asm` count only when `config/AWRE/asm_functions.txt` lists them. Bytes count",
        "code only, including code in dsd gap units that has no function (issue 0071).",
        "",
        *header("Code"),
        row("all", measures(report["measures"])),
        "",
        "## By module",
        "",
        "ARM9 is main, ITCM and the overlays; ARM7 is `arm7.bin`.",
        "",
        *header("Module"),
    ]
    for mod in sorted(by_module, key=lambda k: (order.index(k) if k in order else len(order), k)):
        if by_module[mod][0]:
            lines.append(row(mod, by_module[mod]))
    lines += ["", "## By kind of code", "", "Gap units are code and data that dsd delinks with no source file.",
              "", *header("Kind")]
    for area in [a for a, _ in AREAS] + [GAP]:
        if by_area.get(area, [0])[0]:
            lines.append(row(area, by_area[area]))

    lines += ["", "## Below 100%", "", "Symbols in code that the report scores below 100%, by module.", ""]
    if below:
        lines += ["| Module | Unit | Symbol | Bytes | Match |", "|---|---|---|---:|---:|"]
        key = lambda b: (order.index(b[0]) if b[0] in order else len(order), b[1], b[2])  # noqa: E731
        lines += [f"| {m} | `{u}` | {s} | {n} | {int(p * 100) / 100:.2f}% |" for m, u, s, n, p in sorted(below, key=key)]
    else:
        lines.append("None.")
    if asm:
        lines += ["", "Functions written as `asm` that the report does not count:", "",
                  "| Module | File | Function |", "|---|---|---|"]
        lines += [f"| {m} | `{p}` | {n} |" for m, p, n in asm]
    return "\n".join(lines) + "\n"


def main() -> int:
    if len(sys.argv) not in (1, 3) or (len(sys.argv) == 3 and sys.argv[1] != "--write"):
        sys.exit(__doc__)
    import decomp_dev_report

    path = decomp_dev_report.ARTIFACT / "report.json"
    rel = path.relative_to(ROOT).as_posix()
    if not path.exists():
        sys.exit(f"progress.py: {rel} is missing; run `make release` (or `make decomp-dev-report`) first")
    text = markdown(json.loads(path.read_text()), rel)
    if len(sys.argv) == 1:
        sys.stdout.write(text)
        return 0
    out = Path(sys.argv[2])
    for dest, body in ((out, text), (out.with_suffix(".json"), path.read_text())):
        if not dest.exists() or dest.read_text() != body:
            dest.write_text(body)
    return 0


if __name__ == "__main__":
    sys.exit(main())
