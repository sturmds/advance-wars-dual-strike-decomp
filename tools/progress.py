#!/usr/bin/env python3
"""Decompilation progress (plan D4).

    progress.py                  print a table and write build/report.json
    progress.py --write FILE     write a Markdown report (PROGRESS.md)

Counts matched functions and bytes from the `complete` files in
delinks.txt, split into game code and SDK/library code, and into C and
assembly (functions written as `asm` in a .c file count as assembly, never
as C). The table also counts address-named symbols (func_/data_), for
information only, and runs objdiff-cli's report into build/report.json.

The Markdown report reads only config/ and src/, so it is fast and gives
the same file for the same tree. hooks/pre-commit writes it on every commit.
"""

import re
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import fn  # noqa: E402

ROOT = fn.ROOT
ASM_FUNC = re.compile(r"^\s*asm\s+[\w\s\*]*?\b(\w+)\s*\(", re.M)
AREAS = ("game", "SDK/library")


def asm_names() -> set[str]:
    out = set()
    for mod in fn.modules().values():
        for path, _, _, complete in mod.files:
            if complete and (ROOT / path).exists():
                out.update(ASM_FUNC.findall((ROOT / path).read_text()))
    return out


def count():
    """{(module, area): {"total"|"C"|"asm": [funcs, bytes]}}"""
    asm, done = asm_names(), fn.matched_names()
    out = {}
    for mod in fn.modules().values():
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
    for mod in fn.modules().values():
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
        "| Code | Functions in C | Bytes in C | % of bytes | Functions in asm | Bytes in asm |",
        "|---|---:|---:|---:|---:|---:|",
    ]
    for area in AREAS:
        r = merge(v for (_, a), v in rows.items() if a == area)
        lines.append(f"| {area} | {r['C'][0]} / {r['total'][0]} | {r['C'][1]} / {r['total'][1]} "
                     f"| {pct(r['C'][1], r['total'][1])} | {r['asm'][0]} | {r['asm'][1]} |")
    r = merge(rows.values())
    lines.append(f"| all | {r['C'][0]} / {r['total'][0]} | {r['C'][1]} / {r['total'][1]} "
                 f"| {pct(r['C'][1], r['total'][1])} | {r['asm'][0]} | {r['asm'][1]} |")

    lines += [
        "",
        "## By module",
        "",
        "Rows with nothing done are left out, but count in the totals.",
        "",
        "| Module | Code | Functions in C | Bytes in C | % of bytes | Functions in asm |",
        "|---|---|---:|---:|---:|---:|",
    ]
    order, shown = list(fn.modules()), set()
    for (mod, area), r in sorted(rows.items(), key=lambda kv: (order.index(kv[0][0]), AREAS.index(kv[0][1]))):
        if r["C"][0] or r["asm"][0]:
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
    subprocess.run(fn.ninja("objdiff.json"), cwd=ROOT, check=True, capture_output=True)
    subprocess.run([fn.OBJDIFF, "report", "generate", "-p", ".", "-o", "build/report.json"],
                   cwd=ROOT, check=True, capture_output=True)
    rows = count()
    print(f"{'':<18}{'functions':>12}{'bytes':>12}{'of code':>10}")
    for area in AREAS:
        r = merge(v for (_, a), v in rows.items() if a == area)
        for lang in ("C", "asm"):
            n, b = r[lang]
            print(f"{area + ' ' + lang:<18}{n:>12}{b:>12}{100 * b / r['total'][1]:>9.3f}%")
        print(f"{area + ' total':<18}{r['total'][0]:>12}{r['total'][1]:>12}")
    unnamed = named = 0
    for mod in fn.modules().values():
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
