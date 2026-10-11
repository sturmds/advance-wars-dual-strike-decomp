#!/usr/bin/env python3
"""Find words that look like addresses but have no relocation (issue 0012).

A shifted build (code or data inserted, so later addresses move) only works
if every pointer is a relocation: the linker rewrites relocations, never a
plain number. dsd found most pointers when it made relocs.txt; this lists
the 4-byte words it may have missed.

It reads every ARM9 module's .rodata and .data, and the words in .text that
are not instructions: the literal pool at the end of each function (the
constants a function loads with `ldr rX, =...`; pool offsets from
build/fnindex.json) and data between functions. A word is a candidate
when its value falls inside a module's address range (main, itcm, dtcm or an
overlay, .bss included) and no relocation covers its address. Each candidate
gets a confidence:

  high    the value is the start of a known symbol (or a Thumb function's
          address + 1), and a relocated word sits 4 or 8 bytes away (it
          looks like one more entry of a pointer table)
  medium  the value is the start of a known symbol, or it is 4-aligned with
          a relocated neighbour
  low     anything else in range: often two 16-bit numbers whose upper half
          happens to be 0x01ff-0x0235, or 0x027c-0x027d

Usage:
  scan_pointers.py                      counts by module, section and confidence
  scan_pointers.py --list high          one line per candidate of that level
  scan_pointers.py --list all --module ov000 --tsv FILE

Needs extract/ and build/fnindex.json (`make fnindex`). Reads only.
"""

import argparse
import json
import sys
from collections import Counter

import modmap

LEVELS = ("high", "medium", "low")


def instruction_ranges(index_path) -> dict[str, list[tuple[int, int]]]:
    """Per module, the sorted [start, end) ranges of function instructions,
    literal pools left out."""
    ranges: dict[str, list[tuple[int, int]]] = {}
    for f in json.load(open(index_path))["functions"]:
        addr, pool = int(f["addr"]), int(f["pool"])
        ranges.setdefault(f["module"], []).append((addr, addr + pool))
    return {m: sorted(r) for m, r in ranges.items()}


def code_words(start: int, end: int, insns: list[tuple[int, int]]):
    """Aligned word addresses in [start, end) outside every instruction
    range, each with ".text pool" (right after a function) or ".text other"
    (between functions: crt0's module parameters, for example)."""
    i = 0
    after_function = False
    addr = (start + 3) & ~3
    while addr + 4 <= end:
        while i < len(insns) and insns[i][1] <= addr:
            i += 1
            after_function = True
        if i < len(insns) and insns[i][0] < addr + 4:
            addr = (insns[i][1] + 3) & ~3
            after_function = True
            continue
        yield addr, ".text pool" if after_function else ".text other"
        addr += 4


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--list", choices=LEVELS + ("all",), help="print candidates of this level")
    ap.add_argument("--module", help="only this module (main, itcm, dtcm, ov000...)")
    ap.add_argument("--tsv", help="write candidates to this file instead of stdout")
    args = ap.parse_args()

    index = modmap.ROOT / "build/fnindex.json"
    if not index.exists():
        raise SystemExit("error: build/fnindex.json is missing, run `make fnindex` first")
    mods = modmap.load()
    insns = instruction_ranges(index)

    # Symbol starts in each module, for "the value is a known symbol".
    starts: dict[str, set[int]] = {}
    for m in mods.values():
        s = starts.setdefault(m.name, set())
        for sym in m.symbols:
            s.add(sym.addr)
            if sym.kind.startswith("function(thumb"):
                s.add(sym.addr | 1)
    names = {}
    for m in mods.values():
        for sym in m.symbols:
            names.setdefault((m.name, sym.addr), sym.name)
            if sym.kind.startswith("function(thumb"):
                names.setdefault((m.name, sym.addr | 1), sym.name + "+1")

    rows = []
    for m in mods.values():
        if args.module and m.name != args.module:
            continue
        words = []
        for sec in m.sections:
            end = min(sec.end, m.init_end)
            if sec.kind in ("rodata", "data"):
                words += [(a, sec.name) for a in range((sec.start + 3) & ~3, end - 3, 4)]
            elif sec.kind == "code":
                words += list(code_words(sec.start, end, insns.get(m.name, [])))
        for addr, section in words:
            if addr in m.relocs:
                continue
            value = m.word(addr)
            targets = modmap.modules_at(mods, value & ~1)
            if not targets:
                continue
            symbol = [t.name for t in targets if value in starts[t.name]]
            neighbour = any(addr + d in m.relocs for d in (-8, -4, 4, 8))
            if symbol and neighbour:
                level = "high"
            elif symbol or (value % 4 == 0 and neighbour):
                level = "medium"
            else:
                level = "low"
            target_mod = symbol[0] if symbol else targets[0].name
            rows.append((level, m.name, section, addr, value, "/".join(t.name for t in targets),
                         names.get((target_mod, value), ""), neighbour))

    counts = Counter((r[1], r[2], r[0]) for r in rows)
    print(f"{'module':7} {'section':12} {'high':>6} {'medium':>7} {'low':>7}", file=sys.stderr)
    totals = Counter()
    for mod, section in sorted({(k[0], k[1]) for k in counts}):
        n = [counts[(mod, section, lv)] for lv in LEVELS]
        totals.update(dict(zip(LEVELS, n)))
        print(f"{mod:7} {section:12} {n[0]:6} {n[1]:7} {n[2]:7}", file=sys.stderr)
    print(f"{'total':20} {totals['high']:6} {totals['medium']:7} {totals['low']:7}", file=sys.stderr)

    if args.list:
        out = open(args.tsv, "w") if args.tsv else sys.stdout
        print("level\tmodule\tsection\taddress\tvalue\ttarget modules\tsymbol\trelocated neighbour", file=out)
        for level, mod, section, addr, value, tmods, name, nb in rows:
            if args.list in (level, "all"):
                print(f"{level}\t{mod}\t{section}\t{addr:#010x}\t{value:#010x}\t{tmods}\t{name}\t{'yes' if nb else 'no'}",
                      file=out)
    return 0


if __name__ == "__main__":
    sys.exit(main())
