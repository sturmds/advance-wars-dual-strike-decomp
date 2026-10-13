#!/usr/bin/env python3
"""Check the plain C beside each `asm` function (docs/workflow.md, "Portable
C beside asm").

    python3 tools/tests/test_portable_c.py

Every file under src/ with `#ifdef __MWERKS__` is compiled with the host gcc
(`gcc -fsyntax-only -std=c99`), so `__MWERKS__` is not defined and the
`#else` side is what gcc reads. `-iquote include` finds our headers without
hiding the host's <stdio.h> and <stdarg.h> behind ours.

- A file whose `#else` side is `#error "..."` must fail with that message
  and no other error.
- Any other file must compile, and its `#else` side must define every
  function the file writes as `asm`.

No ROM, build or compiler from make setup needed; gcc must be on PATH.
"""

import re
import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent.parent
GCC = ["gcc", "-fsyntax-only", "-std=c99", "-iquote", str(ROOT / "include")]
# Definitions only: a declaration such as `asm void f(void);` names a
# function that another file defines.
ASM_FUNC = re.compile(r"^\s*asm\s+[\w\s\*]*?\b(\w+)\s*\([^;{]*\)\s*\{", re.M)
ERROR = re.compile(r'^#error "(.*)"$', re.M)

failures = []


def check(path: Path) -> None:
    text = path.read_text()
    rel = path.relative_to(ROOT)
    run = subprocess.run(GCC + [str(path)], capture_output=True, text=True)
    errors = [l for l in run.stderr.splitlines() if " error: " in l]
    wanted = ERROR.findall(text)
    if wanted:
        ok = (run.returncode != 0 and len(errors) == 1 and
              errors[0].endswith(f'#error "{wanted[0]}"'))
        print(f"{rel}: {'ok (#error)' if ok else 'FAIL'}")
    else:
        else_side = text.split("#else", 1)[1] if "#else" in text else ""
        missing = [n for n in ASM_FUNC.findall(text)
                   if not re.search(rf"\b{n}\s*\([^;]*\)\s*{{", else_side)]
        ok = run.returncode == 0 and not missing
        print(f"{rel}: {'ok' if ok else 'FAIL'}")
        if missing:
            print(f"  no C definition after #else for: {', '.join(missing)}")
    if not ok:
        failures.append(str(rel))
        print("  " + run.stderr.strip().replace("\n", "\n  "))


def main() -> int:
    if shutil.which("gcc") is None:
        print("test_portable_c: gcc not found on PATH")
        return 1
    paths = sorted(p for p in (ROOT / "src").rglob("*.c")
                   if "#ifdef __MWERKS__" in p.read_text())
    if not paths:
        print("test_portable_c: no file under src/ has #ifdef __MWERKS__")
        return 1
    for path in paths:
        check(path)
    if failures:
        print(f"{len(failures)} failure(s): {', '.join(failures)}")
        return 1
    print(f"all {len(paths)} files ok")
    return 0


if __name__ == "__main__":
    sys.exit(main())
