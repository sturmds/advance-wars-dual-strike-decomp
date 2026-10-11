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

The Markdown report reads only config/ and src/, so it is fast and gives
the same file for the same tree. hooks/pre-commit writes it on every commit.
"""

import json
import re
import subprocess
import sys
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
    """[(path, function names, bytes)] of source files in src/ that are not
    marked complete: started, but not built into the ROM yet."""
    out, listed = [], set()
    for mod in fn.all_modules().values():
        for path, start, end, complete in mod.files:
            listed.add(path)
            if complete or not path.startswith("src/") or not (ROOT / path).exists():
                continue
            funcs = [f for f in mod.funcs if start <= f.addr < end]
            out.append((path, [f.name for f in funcs], sum(f.size for f in funcs)))
    for p in sorted((ROOT / "src").rglob("*.c")):
        path = p.relative_to(ROOT).as_posix()
        if path not in listed:
            out.append((path, [], 0))
    return sorted(out)


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

    lines += [
        "",
        "## Started, not linked",
        "",
        "Source files in `src/` that the build does not use yet (not marked `complete`).",
        "",
    ]
    files = unlinked()
    if files:
        lines += ["| File | Functions | Bytes |", "|---|---|---:|"]
        for path, names, size in files:
            lines.append(f"| `{path}` | {', '.join(names) or 'no delinks entry'} | {size} |")
    else:
        lines.append("None.")
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
