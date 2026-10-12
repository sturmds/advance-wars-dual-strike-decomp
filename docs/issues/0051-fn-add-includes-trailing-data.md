# 0051: `fn.py add` includes data after a function in its delinks.txt range

- **Status:** closed
- **Area:** tools
- **Opened:** 2026-10-11
- **Milestone:** M1

## Problem

`fn.py add` sets the end of a function's `delinks.txt` entry at the next symbol. When bytes after the function have no symbol (a table or literal pool the symbol list does not name), they fall inside the range. The C file does not produce them, so the build no longer matches until the range is cut back by hand.

Seen twice in batch 20:

- `Screen_IsTextBg`: its table at 0x01ff8f64 sat inside the entry.
- `RangeMap_FillMoves`: the gap 0x01ffb95c-0x01ffbc44 after it sat inside the entry.

Again on 2026-10-11 (b26-crt0): `fn.py add` for `NitroStartUp` (`func_02000afc`) extended the entry to 0x02000b24, over `BuildInfo`, the SDK module-parameter block that follows the function. The entry had to be cut back to 0x02000b00 by hand.

## Workaround

After `fn.py add`, look for a gap between the last instruction and the next symbol, and set the entry's end to the function's last byte (or add a symbol for the data).

## Ideas

- Stop the range at the last instruction of the function (dsd knows where the code ends) instead of the next symbol.
- When the next symbol is more than a literal pool away, print the gap and ask for a decision.
- Related: 0041 (no tool shows data structures).

## Fix (2026-10-12)

`fn.py add` ends the range at the function's size in `symbols.txt`, or at the next function, data or bss symbol if that comes first (`fn.next_start`). When the next symbol starts within the padding to a 4-byte boundary, the range takes the padding in. Unnamed bytes after the function stay out of the range, and `add` prints them (`fn.trailing_gap`): they may be data, or the function's literal pool or code when dsd sized the symbol too small (0060). `trivial.py` and `propagate.py` add files through `fn.py add`, so they get the same range; `propagate.py` no longer skips functions with data after them.

Tests: `make test` (`tools/tests/test_fnadd.py`).
