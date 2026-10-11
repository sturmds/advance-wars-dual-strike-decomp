#!/usr/bin/env python3
"""Add linker-absolute symbols to a linker script that dsd lcf wrote.

usage: lcf_absolute.py LCF NAME=VALUE...

Writes `NAME = VALUE;` at the top of the SECTIONS block, once per name.
tools/configure.py runs it after `dsd lcf` with the list in
ARM9_ABSOLUTE_SYMBOLS.
"""

# Based on khdays-decomp tools/configure.py (add_absolute_symbols)
# (CC0 1.0, commit ba2de370; see docs/third-party/khdays-decomp.md).

import sys
from pathlib import Path

MARKER = "SECTIONS {\n"


def add(text: str, symbols: dict[str, int]) -> str:
    """text with one assignment per symbol that is not there yet."""
    lines = [f"    {name} = {value:#010x};" for name, value in sorted(symbols.items())]
    missing = [line for line in lines if line not in text]
    if not missing:
        return text
    at = text.index(MARKER) + len(MARKER)
    return text[:at] + "\n".join(missing) + "\n" + text[at:]


def main() -> int:
    lcf, *pairs = sys.argv[1:]
    symbols = {}
    for pair in pairs:
        name, value = pair.split("=")
        symbols[name] = int(value, 0)
    path = Path(lcf)
    text = path.read_text()
    new = add(text, symbols)
    if new != text:
        path.write_text(new)
    return 0


if __name__ == "__main__":
    sys.exit(main())
