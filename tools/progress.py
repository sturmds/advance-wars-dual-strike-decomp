#!/usr/bin/env python3
"""Decompilation progress (plan D4).

    progress.py                  print a table and write build/report.json
    progress.py --write FILE     write a Markdown report (PROGRESS.md)

Counts matched functions and bytes from the `complete` files in
delinks.txt, split by CPU (ARM9: main, ITCM, overlays; ARM7: the `arm7`
module), into game code and SDK/library code, and into C and assembly
(functions written as `asm` in a .c file count as assembly, never as C).
ARM7 functions count in the totals from the day they were delinked, with
or without C. The rows labelled `game`, `SDK/library` and `all` add both
CPUs (the loop reads `all`); per-CPU rows are labelled `ARM9 ...` and
`ARM7 ...`. The table also counts address-named symbols (func_/data_), for
information only, and runs objdiff-cli's reports: build/report.json for
ARM9 and build/arm7/report.json for ARM7, with the split counts in
build/progress.json.

The Markdown report reads config/, src/ and include/, and gives the same
file for the same tree. For the partial functions (started in C, not linked)
it runs `fn.py diff` on each one (about 6 seconds in all); without extract/
and build/ those columns show "-". hooks/pre-commit writes it on every commit, after `make release`.
"""

import json
import re
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import fn  # noqa: E402

ROOT = fn.ROOT
ASM_FUNC = re.compile(r"^\s*asm\s+[\w\s\*]*?\b(\w+)\s*\(", re.M)
AREAS = ("game", "SDK/library")
# Each CPU's modules. The Download Play child (issue 0049) would add its own
# two CPUs here once it is delinked.
CPUS = {"ARM9": lambda mod: mod != "arm7", "ARM7": lambda mod: mod == "arm7"}


def asm_names() -> set[str]:
    out = set()
    for mod in fn.all_modules().values():
        for path, _, _, complete in mod.files:
            if complete and (ROOT / path).exists():
                out.update(ASM_FUNC.findall((ROOT / path).read_text()))
    return out


def count():
    """{(module, area): {"total"|"C"|"asm": [funcs, bytes]}}"""
    asm, done = asm_names(), fn.matched_names()
    out = {}
    for mod in fn.all_modules().values():
        for f in mod.funcs:
            area = "game" if fn.is_game(f) else "SDK/library"
            row = out.setdefault((mod.name, area), {k: [0, 0] for k in ("total", "C", "asm")})
            keys = ["total"]
            if f.name in done:
                keys.append("asm" if f.name in asm else "C")
            for k in keys:
                row[k][0] += 1
                row[k][1] += f.size
    return out


def merge(rows):
    out = {k: [0, 0] for k in ("total", "C", "asm")}
    for row in rows:
        for k, (n, b) in row.items():
            out[k][0] += n
            out[k][1] += b
    return out


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


class Score:
    """One function's `fn.py diff`: objdiff's match percentage, the rows of
    the aligned listing, and how many of them differ (fn.py's count, which
    leaves out = rows)."""

    def __init__(self, percent: float, rows: int, differ: int):
        self.percent, self.rows, self.differ = percent, rows, differ


def diff_score(name: str) -> Score | None:
    out = subprocess.run([sys.executable, str(ROOT / "tools/fn.py"), "diff", "--color", "never", name],
                         cwd=ROOT, capture_output=True, text=True).stdout
    score = fn.parse_diff(out)
    if score is None or score.percent is None:
        return None
    return Score(score.percent, len(score.rows), score.differ)


def scores(files) -> dict:
    """{(module, name): Score} for every function in the unlinked files, from
    `fn.py diff` (objdiff, aligned, the percentage the parked list uses).
    Missing for a function that does not compile or without extract/."""
    jobs = [(mod, f.name) for _, mod, funcs in files for f in funcs]
    with ThreadPoolExecutor(4) as pool:
        got = pool.map(lambda job: (job, diff_score(job[1])), jobs)
        return {k: r for k, r in got if r is not None}


# Bands of differing instructions for the partials summary.
BANDS = ((1, 2), (3, 10), (11, 50), (51, None))


def partials_section() -> list[str]:
    files = unlinked()
    res = scores(files)
    funcs = [(path, mod, f) for path, mod, fs in files for f in fs]
    lines = [
        "## Partial functions",
        "",
        "Functions in C files that the build does not use yet (not marked `complete`), because",
        "they do not match. Each one is compiled and compared with the original by `fn.py diff`:",
        "\"Differ\" counts the rows of its aligned listing that differ (an instruction only one side",
        "has counts as one row; a relocation naming another symbol at the same address does not",
        "count). \"Match\" is objdiff's percentage. The list ranks by \"Differ\", then \"Match\":",
        "objdiff's score charges one wrong opcode as much as 12 wrong registers (issue 0067).",
        "",
    ]
    if not funcs:
        lines.append("None.")
    else:
        got = [res[(m, f.name)] for _, m, f in funcs if (m, f.name) in res]
        rows = sum(r.rows for r in got)
        same = rows - sum(r.differ for r in got)
        lines += ["| Code | Functions | Bytes | Rows that match |", "|---|---:|---:|---:|"]
        for cpu, has in CPUS.items():
            for area in AREAS:
                part = [(m, f) for _, m, f in funcs if has(m) and (fn.is_game(f) == (area == "game"))]
                if part:
                    sc = [res[(m, f.name)] for m, f in part if (m, f.name) in res]
                    n = sum(r.rows for r in sc)
                    ok = n - sum(r.differ for r in sc)
                    lines.append(f"| {cpu} {area} | {len(part)} | {sum(f.size for _, f in part)} "
                                 f"| {f'{ok} / {n} ({pct(ok, n)})' if sc else '-'} |")
        lines += [
            f"| all | {len(funcs)} | {sum(f.size for _, _, f in funcs)} "
            f"| {f'{same} / {rows} ({pct(same, rows)})' if got else '-'} |",
            "",
            "| Rows that differ | Functions | Bytes |",
            "|---|---:|---:|",
        ]
        bands = [(f"{lo} to {hi}" if hi else f"more than {lo - 1}",
                  lambda r, lo=lo, hi=hi: lo <= r.differ and (hi is None or r.differ <= hi)) for lo, hi in BANDS]
        bands.insert(0, ("none (matches; mark it complete)", lambda r: r.differ == 0))
        for label, test in bands:
            band = [f for _, m, f in funcs if (r := res.get((m, f.name))) and test(r)]
            if band or label[0].isdigit() or label.startswith("more"):
                lines.append(f"| {label} | {len(band)} | {sum(f.size for f in band)} |")
        missing = [f for _, m, f in funcs if (m, f.name) not in res]
        if missing and got:
            lines.append(f"| not scored (does not compile) | {len(missing)} | {sum(f.size for f in missing)} |")
        lines += ["", "Closest first.", "", "| File | Function | Bytes | Differ | Match |", "|---|---|---:|---:|---:|"]

        def key(row):
            r = res.get((row[1], row[2].name))
            return (r is None, *(fn.rank(r.differ, r.percent) if r else (0, 0.0)), row[0], row[2].name)

        for path, mod, f in sorted(funcs, key=key):
            r = res.get((mod, f.name))
            match, differ = (f"{r.percent:.2f}%", str(r.differ)) if r else ("-", "-")
            lines.append(f"| `{path}` | {f.name} | {f.size} | {differ} | {match} |")
    orphans = [path for path, mod, _ in files if mod is None]
    if orphans:
        lines += ["", "C files with no delinks entry: " + ", ".join(f"`{p}`" for p in orphans) + "."]
    return lines


def totals(rows):
    """[(label, merged counts)] for the totals table: each CPU by area, each
    CPU's sum, then both CPUs by area and in all. A CPU with no code in an
    area (ARM7 has no game code) gets no row for it."""
    out = []
    for cpu, has in CPUS.items():
        mine = {k: v for k, v in rows.items() if has(k[0])}
        for area in AREAS:
            part = [v for (_, a), v in mine.items() if a == area]
            if part:
                out.append((f"{cpu} {area}", merge(part)))
        out.append((f"{cpu} all", merge(mine.values())))
    for area in AREAS:
        out.append((area, merge(v for (_, a), v in rows.items() if a == area)))
    out.append(("all", merge(rows.values())))
    return out


def pct(part: int, whole: int) -> str:
    return f"{100 * part / whole:.2f}%" if whole else "-"


def markdown() -> str:
    rows = count()
    lines = [
        "# Decompilation progress",
        "",
        "Written by `tools/progress.py --write PROGRESS.md` from `hooks/pre-commit`; do not edit.",
        "A function counts as done when its file is marked `complete` in `delinks.txt`, so the",
        "build links it from `src/`. Functions written as `asm` inside a .c file count separately.",
        "",
        "## Totals",
        "",
        "ARM9 is main, ITCM and the overlays; ARM7 is `arm7.bin`. `game`, `SDK/library` and `all`",
        "add both CPUs.",
        "",
        "| Code | Functions in C | Bytes in C | % of bytes | Functions in asm | Bytes in asm |",
        "|---|---:|---:|---:|---:|---:|",
    ]
    for label, r in totals(rows):
        lines.append(f"| {label} | {r['C'][0]} / {r['total'][0]} | {r['C'][1]} / {r['total'][1]} "
                     f"| {pct(r['C'][1], r['total'][1])} | {r['asm'][0]} | {r['asm'][1]} |")

    lines += [
        "",
        "## By module",
        "",
        "Rows with nothing done are left out, but count in the totals; ARM7's row is always shown.",
        "",
        "| Module | Code | Functions in C | Bytes in C | % of bytes | Functions in asm |",
        "|---|---|---:|---:|---:|---:|",
    ]
    order, shown = list(fn.all_modules()), set()
    for (mod, area), r in sorted(rows.items(), key=lambda kv: (order.index(kv[0][0]), AREAS.index(kv[0][1]))):
        if r["C"][0] or r["asm"][0] or mod == "arm7":
            shown.add(mod)
            lines.append(f"| {mod} | {area} | {r['C'][0]} / {r['total'][0]} | {r['C'][1]} / {r['total'][1]} "
                         f"| {pct(r['C'][1], r['total'][1])} | {r['asm'][0]} |")
    empty = [m for m in order if m not in shown and any(k[0] == m for k in rows)]
    lines += ["", f"Modules with nothing done: {', '.join(empty) or 'none'}."]

    lines += [""] + partials_section()
    return "\n".join(lines) + "\n"


def table() -> int:
    subprocess.run(fn.ninja("objdiff.json", "build/arm7/objdiff.json"), cwd=ROOT, check=True, capture_output=True)
    for project, out in ((".", "build/report.json"), ("build/arm7", "build/arm7/report.json")):
        subprocess.run([fn.OBJDIFF, "report", "generate", "-p", project, "-o", out],
                       cwd=ROOT, check=True, capture_output=True)
    rows = count()
    split = {}
    print(f"{'':<24}{'functions':>12}{'bytes':>12}{'of code':>10}")
    for label, r in totals(rows):
        split[label] = r
        for lang in ("C", "asm"):
            n, b = r[lang]
            print(f"{label + ' ' + lang:<24}{n:>12}{b:>12}{pct(b, r['total'][1]):>10}")
        print(f"{label + ' total':<24}{r['total'][0]:>12}{r['total'][1]:>12}")
    (ROOT / "build/progress.json").write_text(json.dumps(
        {label: {k: {"functions": n, "bytes": b} for k, (n, b) in r.items()} for label, r in split.items()},
        indent=1) + "\n")
    unnamed = named = 0
    for mod in fn.all_modules().values():
        for name in mod.syms.values():
            if re.match(r"(func|data)_", name):
                unnamed += 1
            else:
                named += 1
    print(f"\nsymbols with address names: {unnamed} (named: {named})")
    return 0


def main() -> int:
    if len(sys.argv) == 3 and sys.argv[1] == "--write":
        out = Path(sys.argv[2])
        text = markdown()
        if not out.exists() or out.read_text() != text:
            out.write_text(text)
        return 0
    if len(sys.argv) != 1:
        sys.exit(__doc__)
    return table()


if __name__ == "__main__":
    sys.exit(main())
