#!/usr/bin/env python3
"""Add linker-defined symbols to a linker script that dsd lcf wrote.

usage: lcf_absolute.py LCF NAME=VALUE...

VALUE is one of:
  a number      `NAME = VALUE;` at the top of the SECTIONS block (a
                link-time constant such as a stack size);
  @REGION       a section placed in the MEMORY region REGION, whose start,
                rounded up to 32 bytes, is NAME. dsd's script ends with
                `SPACE : ORIGIN = AFTER(ARM9, OV000, ...)`, so `@SPACE` is
                the first address past main and every overlay: the start of
                the main arena. `ADDR(SPACE)` cannot give it: mwldarm 2.0-72
                links that as 0, because no section is placed in SPACE;
  a symbol      `NAME = SYMBOL;` at the end of the SECTIONS block, after
                the sections that define SYMBOL (mwldarm rejects a symbol
                used before its section).

Each is written once. tools/configure.py runs it after `dsd lcf` with
ARM9_ABSOLUTE_SYMBOLS and ARM9_LAYOUT_SYMBOLS.
"""

# Based on khdays-decomp tools/configure.py (add_absolute_symbols)
# (CC0 1.0, commit ba2de370; see docs/third-party/khdays-decomp.md).

import sys
from pathlib import Path

MARKER = "SECTIONS {\n"


def _insert_top(text: str, lines: list[str]) -> str:
    missing = [line for line in lines if line not in text]
    if not missing:
        return text
    at = text.index(MARKER) + len(MARKER)
    return text[:at] + "\n".join(missing) + "\n" + text[at:]


def _insert_end(text: str, blocks: list[str]) -> str:
    missing = [block for block in blocks if block not in text]
    if not missing:
        return text
    at = text.rindex("}")  # the end of the SECTIONS block, the last block
    return text[:at] + "".join(missing) + text[at:]


def add(text: str, symbols: dict[str, int | str]) -> str:
    """text with each symbol defined once."""
    numbers = {k: v for k, v in symbols.items() if isinstance(v, int)}
    text = _insert_top(text, [f"    {name} = {value:#010x};" for name, value in sorted(numbers.items())])
    blocks = []
    for name, value in sorted(symbols.items()):
        if isinstance(value, int):
            continue
        if value.startswith("@"):
            region = value[1:]
            blocks.append(f"    .{name} : {{\n        . = ALIGN(32);\n        {name} = .;\n    }} > {region}\n")
        else:
            blocks.append(f"    {name} = {value};\n")
    return _insert_end(text, blocks)


def parse(value: str) -> int | str:
    try:
        return int(value, 0)
    except ValueError:
        return value


def main() -> int:
    lcf, *pairs = sys.argv[1:]
    symbols = {}
    for pair in pairs:
        name, value = pair.split("=")
        symbols[name] = parse(value)
    path = Path(lcf)
    text = path.read_text()
    new = add(text, symbols)
    if new != text:
        path.write_text(new)
    return 0


if __name__ == "__main__":
    sys.exit(main())
