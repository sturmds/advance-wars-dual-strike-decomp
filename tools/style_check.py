#!/usr/bin/env python3
"""Check prose against docs/style/banned.txt, docs/style/claudeisms.csv and the "X is the Y" ban.

Usage:
    style_check.py [--quiet-warnings] FILE...   check files ("-" reads stdin)

Exits 1 if any banned word appears. Claudeisms and possible "X is the Y"
lines are printed as warnings and do not change the exit code, because many
of those words are normal in this project and the pattern check is a guess.
"""

import argparse
import csv
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
BANNED = ROOT / "docs/style/banned.txt"
CLAUDEISMS = ROOT / "docs/style/claudeisms.csv"

# "the hash is the test", "this file is the source of truth". A determiner,
# one to three words, "is the", then a noun. Matches plenty of fine sentences
# too, so it only warns.
X_IS_THE_Y = re.compile(
    r"\b(?:the|this|that|our|its|a|an)\s+(?:[\w`'./-]+\s+){0,2}?[\w`'./-]+\s+is\s+the\s+[\w`'./-]+",
    re.IGNORECASE,
)


def word_pattern(entry: str) -> re.Pattern:
    stem = entry.rstrip("*")
    tail = r"[\w-]*" if entry.endswith("*") else ""
    return re.compile(r"(?<![\w-])" + re.escape(stem) + tail + r"(?![\w-])", re.IGNORECASE)


def load_banned() -> list[tuple[str, re.Pattern]]:
    entries = []
    for line in BANNED.read_text().splitlines():
        line = line.strip()
        if line and not line.startswith("#"):
            entries.append((line, word_pattern(line)))
    return entries


def load_claudeisms() -> list[tuple[str, re.Pattern]]:
    with CLAUDEISMS.open(newline="") as f:
        words = [row["word"] for row in csv.DictReader(f)]
    return [(w, word_pattern(w)) for w in words]


URL = re.compile(r"\S+://\S+")
TRAILER = re.compile(r"^(Co-Authored-By|Claude-Session|Signed-off-by): ", re.IGNORECASE)


def check(name: str, text: str, banned, warn, show_warnings: bool) -> int:
    errors = 0
    for lineno, line in enumerate(text.splitlines(), 1):
        if TRAILER.match(line):
            continue
        line = URL.sub("", line)
        for entry, pat in banned:
            for m in pat.finditer(line):
                print(f"{name}:{lineno}: error: banned word '{m.group(0)}' ({entry})")
                errors += 1
        if not show_warnings:
            continue
        for word, pat in warn:
            for m in pat.finditer(line):
                print(f"{name}:{lineno}: warning: claudeism '{m.group(0)}'")
        for m in X_IS_THE_Y.finditer(line):
            print(f"{name}:{lineno}: warning: possible 'X is the Y': '{m.group(0)}'")
    return errors


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--quiet-warnings", action="store_true", help="print errors only")
    ap.add_argument("files", nargs="+")
    args = ap.parse_args()

    banned = load_banned()
    banned_stems = {e.rstrip("*") for e, _ in banned}
    warn = [(w, p) for w, p in load_claudeisms()
            if not any(w.startswith(s) for s in banned_stems)]

    errors = 0
    for name in args.files:
        text = sys.stdin.read() if name == "-" else Path(name).read_text(errors="replace")
        errors += check(name, text, banned, warn, not args.quiet_warnings)
    return 1 if errors else 0


if __name__ == "__main__":
    sys.exit(main())
