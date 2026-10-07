#!/usr/bin/env python3
"""Print decompilation progress (plan D4).

Counts matched functions and bytes from the `complete` files in
delinks.txt, split into game code and SDK/library code, and into C and
assembly (functions written as `asm` in a .c file count as assembly, never
as C). Also counts address-named symbols (func_/data_), for information
only. objdiff-cli's own report goes to build/report.json.
"""

import re
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import fn  # noqa: E402

ROOT = fn.ROOT
ASM_FUNC = re.compile(r"^\s*asm\s+[\w\s\*]*?\b(\w+)\s*\(", re.M)


def main() -> int:
    subprocess.run(["ninja", "objdiff.json"], cwd=ROOT, check=True, capture_output=True)
    subprocess.run([fn.OBJDIFF, "report", "generate", "-p", ".", "-o", "build/report.json"],
                   cwd=ROOT, check=True, capture_output=True)

    asm = set()
    for mod in fn.modules().values():
        for path, _, _, complete in mod.files:
            if complete and (ROOT / path).exists():
                asm.update(ASM_FUNC.findall((ROOT / path).read_text()))

    rows = {}  # (area, lang) -> [funcs, bytes]
    totals = {"game": [0, 0], "SDK/library": [0, 0]}
    done = fn.matched_names()
    unnamed = named = 0
    for mod in fn.modules().values():
        for f in mod.funcs:
            area = "game" if fn.is_game(f) else "SDK/library"
            totals[area][0] += 1
            totals[area][1] += f.size
            if f.name in done:
                key = (area, "asm" if f.name in asm else "C")
                r = rows.setdefault(key, [0, 0])
                r[0] += 1
                r[1] += f.size
        for name in mod.syms.values():
            if re.match(r"(func|data)_", name):
                unnamed += 1
            else:
                named += 1

    print(f"{'':<18}{'functions':>12}{'bytes':>12}{'of code':>10}")
    for area in ("game", "SDK/library"):
        for lang in ("C", "asm"):
            n, b = rows.get((area, lang), [0, 0])
            print(f"{area + ' ' + lang:<18}{n:>12}{b:>12}{100 * b / totals[area][1]:>9.3f}%")
        print(f"{area + ' total':<18}{totals[area][0]:>12}{totals[area][1]:>12}")
    print(f"\nsymbols with address names: {unnamed} (named: {named})")
    return 0


if __name__ == "__main__":
    sys.exit(main())
