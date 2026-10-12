#!/usr/bin/env python3
"""Format the C in src/ and include/ with clang-format, or check it.

    tools/format.py           rewrite every file that needs it
    tools/format.py --check   list the files that need it; exit 1 if any
    tools/format.py FILE...   only these files (either mode)

The style is in .clang-format. Files that define an `asm` function are
skipped: clang-format reads ARM assembly as C and breaks it (`#4` looks
like a preprocessor line). clang-format comes from tools.lock.
"""

import argparse
import os
import re
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
CLANG_FORMAT = ROOT / ".cache/tools/clang-format/clang_format/data/bin/clang-format"
ASM_FUNCTION = re.compile(r"^(static\s+)?asm\b", re.M)
BATCH = 200


def tracked_sources() -> list[Path]:
    out = subprocess.run(["git", "ls-files", "-z", "--", "src", "include"],
                         cwd=ROOT, check=True, capture_output=True, text=True).stdout
    return [ROOT / p for p in out.split("\0") if p.endswith((".c", ".h"))]


def has_asm_function(path: Path) -> bool:
    return bool(ASM_FUNCTION.search(path.read_text(encoding="utf-8", errors="replace")))


def run(batch: list[Path], check: bool) -> list[str]:
    """Format one batch in place, or return the files in it that need formatting."""
    args = [str(CLANG_FORMAT), "--style=file"]
    args += ["--dry-run", "-Werror", "--ferror-limit=1"] if check else ["-i"]
    r = subprocess.run(args + [str(p) for p in batch], cwd=ROOT,
                       capture_output=True, text=True)
    if not check:
        if r.returncode:
            sys.exit(f"error: clang-format failed:\n{r.stderr}")
        return []
    # Each problem starts with "path:line:col: error: code should be clang-formatted".
    bad = re.findall(r"^(.+?):\d+:\d+: error:", r.stderr, re.M)
    if r.returncode and not bad:
        sys.exit(f"error: clang-format failed:\n{r.stderr}")
    return bad


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--check", action="store_true", help="report, do not rewrite")
    ap.add_argument("files", nargs="*", type=Path)
    a = ap.parse_args()

    if not CLANG_FORMAT.exists():
        sys.exit(f"error: {CLANG_FORMAT} is missing; run `make setup`")
    files = [p.resolve() for p in a.files] if a.files else tracked_sources()
    files = [p for p in files if p.suffix in (".c", ".h") and not has_asm_function(p)]
    batches = [files[i:i + BATCH] for i in range(0, len(files), BATCH)]
    with ThreadPoolExecutor(os.cpu_count() or 1) as pool:
        bad = sorted({b for r in pool.map(lambda b: run(b, a.check), batches) for b in r})

    if bad:
        for b in bad:
            print(f"format: {os.path.relpath(b, ROOT)} is not formatted", file=sys.stderr)
        print(f"format: {len(bad)} file(s); run `make format`", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
