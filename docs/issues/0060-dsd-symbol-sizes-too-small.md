# 0060: dsd symbol sizes too small for runtime functions, and tails split off as functions

- **Status:** open
- **Area:** tools
- **Opened:** 2026-10-11
- **Milestone:** M1

## Problem

For several functions in the runtime and MSL region, the size in `symbols.txt` ends before the function does. Agents found this only when the diff or the build showed code outside the range:

- `_fmul`: size 0x1ec, real 0x4e0 (b26-rtf0).
- `_fdiv`: size 0x380, real 0x6f0 including a two-word literal pool (b26-rtf0).
- `_dmul`: size 0x2dc, real 0x6f0 (b26-rtd0).
- The float and double compare block `func_02127134`: size 0x9e4, code runs to 0x02127ca0 (b26-rtd1).
- `__dorounddecup`: the trailing `bx lr` became its own symbol `func_021256a4` (size 4) through dsd's empty-function propagation (b27-sdkp1). Merged back by hand to size 0x50. Partials12 (2026-10-10) merged other 4-byte tails the same way.

## Notes

- A check could find more. For each function symbol, look at the bytes between its end and the next symbol's start; if they decode as code (not a literal pool, not zeros), report the symbol. The reverse check also helps: a symbol of size 4 or 8 whose predecessor ends without a return or an unconditional branch.
- It can run on `config/AWRE/arm9/*/symbols.txt` with the disassembly, or on the index `tools/fnindex.py` builds.
- Related: 0051 (`fn.py add` includes trailing data): same area, opposite error.

## Done when

- A tool lists the symbols that end before the next symbol with code in between, and the list for the current symbols is empty or each entry has a comment.
