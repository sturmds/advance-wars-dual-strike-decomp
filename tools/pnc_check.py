#!/usr/bin/env python3
"""Privacy check (tenet 8): no tracked file, path or commit message matches a private pattern.

    pnc_check.py                 check every tracked file and its path
    pnc_check.py --message FILE  check one commit message (hooks/commit-msg)

The patterns are case-insensitive regexes, one per line, in
.git/info/pnc-patterns. That file is never committed: a list of what must
stay private would itself say it. Without the file the check passes and says
so, so a clone without it still builds.
"""

import argparse
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent


def patterns() -> re.Pattern | None:
    git_dir = subprocess.run(["git", "rev-parse", "--git-common-dir"], cwd=ROOT,
                             capture_output=True, text=True, check=True).stdout.strip()
    path = (ROOT / git_dir / "info" / "pnc-patterns").resolve()
    if not path.exists():
        return None
    lines = [l.strip() for l in path.read_text().splitlines() if l.strip() and not l.startswith("#")]
    return re.compile("|".join(f"(?:{l})" for l in lines).encode(), re.I) if lines else None


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--message", type=Path, help="a commit message file")
    a = ap.parse_args()
    pat = patterns()
    if pat is None:
        print("pnc-check: no .git/info/pnc-patterns, skipped")
        return 0
    if a.message:
        text = a.message.read_bytes()
        m = pat.search(text)
        if m:
            print(f"pnc-check: the commit message matches a private pattern ({m.group(0).decode(errors='replace')!r})", file=sys.stderr)
            return 1
        return 0
    files = subprocess.run(["git", "ls-files", "-z"], cwd=ROOT, capture_output=True, check=True).stdout.split(b"\0")
    bad = []
    for f in filter(None, files):
        p = ROOT / f.decode()
        if pat.search(f) or (p.is_file() and not p.is_symlink() and pat.search(p.read_bytes())):
            bad.append(f.decode())
    if bad:
        print("pnc-check: these tracked files match a private pattern:", file=sys.stderr)
        for f in bad:
            print(f"  {f}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
