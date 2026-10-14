#!/usr/bin/env python3
"""Privacy and naming checks (tenets 8 and 10) over tracked files, paths and commit messages.

    pnc_check.py                 check every tracked file and its path
    pnc_check.py --message FILE  check one commit message (hooks/commit-msg)

Tenet 10: a name shaped like a DS library name (OS_Foo, CARDi_Bar, WMFoo,
REG_X) must be in tools/pnc_sdk_names.txt, the names with a public source.
Any other name needs a name of our own. PROGRESS.md and PROGRESS.json are
left out of this part: they are written from names checked elsewhere.

Tenet 8: the patterns are case-insensitive regexes, one per line, in
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
SDK_SHAPE = re.compile(rb"\b(?:OS|FS|GX|GXS|G2|G2S|G3|MI|FX|MTX|VEC|CP|CARD|CTRDG|PXI|SND|WM|WL|MB|PM|TP|SVC|NNS|SPI|RTC|HW|SDK|REG|IC|DC|PAD)"
                       rb"(?:i?_[A-Za-z0-9_]+|[A-Z][a-z][A-Za-z0-9]*)\b")


def allowed_names() -> set[bytes]:
    lines = (ROOT / "tools/pnc_sdk_names.txt").read_bytes().splitlines()
    return {l.strip() for l in lines if l.strip() and not l.startswith(b"#")}


def unsourced(text: bytes, allowed: set[bytes]) -> set[bytes]:
    out = set()
    for m in SDK_SHAPE.finditer(text):
        name = m.group(0)
        base = re.sub(rb"_(Arm7|ARM7)$", b"", name)
        if name not in allowed and base not in allowed:
            out.add(name)
    return out


def patterns() -> re.Pattern | None:
    git_dir = subprocess.run(["git", "rev-parse", "--git-common-dir"], cwd=ROOT,
                             capture_output=True, text=True, check=True).stdout.strip()
    path = (ROOT / git_dir / "info" / "pnc-patterns").resolve()
    if not path.exists():
        return None
    lines = [l.strip() for l in path.read_text().splitlines() if l.strip() and not l.startswith("#")]
    return re.compile("|".join(f"(?:{l})" for l in lines).encode(), re.I) if lines else None


NAME_CHECK_SKIP = {b"tools/pnc_sdk_names.txt", b"tools/pnc_check.py", b"PROGRESS.md", b"PROGRESS.json"}


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--message", type=Path, help="a commit message file")
    a = ap.parse_args()
    pat = patterns()
    allowed = allowed_names()
    if a.message:
        text = a.message.read_bytes()
        m = pat.search(text) if pat else None
        if m:
            print(f"pnc-check: the commit message matches a private pattern ({m.group(0).decode(errors='replace')!r})", file=sys.stderr)
            return 1
        names = unsourced(text, allowed)
        if names:
            print(f"pnc-check: the commit message uses library-style names with no public source: "
                  f"{', '.join(sorted(n.decode() for n in names))} (tenet 10, tools/pnc_sdk_names.txt)", file=sys.stderr)
            return 1
        return 0
    if pat is None:
        print("pnc-check: no .git/info/pnc-patterns, privacy patterns skipped")
    files = subprocess.run(["git", "ls-files", "-z"], cwd=ROOT, capture_output=True, check=True).stdout.split(b"\0")
    bad, names = [], {}
    for f in filter(None, files):
        p = ROOT / f.decode()
        data = p.read_bytes() if p.is_file() and not p.is_symlink() else b""
        if pat and (pat.search(f) or pat.search(data)):
            bad.append(f.decode())
        # PROGRESS.md and PROGRESS.json are written from the report of the names in
        # src/ and config/, which this loop checks already; a landing replays
        # mainline's copy, so a rename would fail here until the hook rewrites it.
        if f not in NAME_CHECK_SKIP:
            for n in unsourced(data, allowed):
                names.setdefault(n.decode(), f.decode())
    if bad:
        print("pnc-check: these tracked files match a private pattern:", file=sys.stderr)
        for f in bad:
            print(f"  {f}", file=sys.stderr)
    if names:
        print("pnc-check: library-style names with no public source (tenet 10); give them names of our own,", file=sys.stderr)
        print("  or add one to tools/pnc_sdk_names.txt with its public source:", file=sys.stderr)
        for n, f in sorted(names.items()):
            print(f"  {n}  ({f})", file=sys.stderr)
    return 1 if bad or names else 0


if __name__ == "__main__":
    sys.exit(main())
