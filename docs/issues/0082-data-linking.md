# 0082: Link data from C

- **Status:** in progress
- **Area:** decomp
- **Opened:** 2026-10-13
- **Milestone:** M2

## Problem

decomp.dev shows all code decompiled and linked, but 94% of the data bytes are still in dsd gap units, which have no source file. Tracked on GitHub as https://github.com/sturmds/advance-wars-dual-strike-decomp/issues/1, with the numbers and the plan (maps, then `.bss`, then `.rodata`, then `.data`).

## Notes

- Maps: the three map tables in ov000 score 100% matched data (188,932 bytes, 6.02% of all data). Tiles are one symbol with relocations into it, units one array per list in the order mwccarm emits them, and 38 false relocations dsd had found in the tables are gone. See `docs/maps.md`, "Build", and `docs/workflow.md`, "Several data objects in one file".
- `.bss`: `tools/bss.py` (`docs/bss-tool.md`). ov004 done: its 608,288 bytes link from `ov004_init3d.c` and `ov004_alloc_state.c`, and linked data is 797,220 bytes (25.4%). arm7 done: its .bss and .sbss (20,532 bytes) link from 23 existing files and 5 new ones, and linked data is 817,752 bytes (26.1%). Left: ov000, ov002, ov003, ov005, ov006, main.
- `.bss` ov000: 93 variables, 576 bytes, in 4 files (3 existing, 1 new, generated); linked data 818,328 bytes (26.1%).
- `.bss` ov002: 43 variables, 224 bytes, in 4 files (2 existing, 2 new, 2 of them generated); linked data 818,552 bytes (26.1%).
- `.bss` ov003: 19 variables, 16,672 bytes, in 7 files (6 existing, 1 new, 1 of them generated); linked data 835,224 bytes (26.6%).
- `.bss` ov005: 13 variables, 64 bytes, in 12 files (12 existing, 0 new, 0 of them generated); linked data 835,288 bytes (26.6%).
- `.bss` ov006: 4 variables, 864 bytes, in 4 files (3 existing, 1 new, 1 of them generated); linked data 836,152 bytes (26.7%).
- `.bss` main 0x0216e020-0x0218ff50: 141 variables, 139,056 bytes, in 17 files (2 existing, 15 new, 13 of them generated); linked data 975,208 bytes (31.1%).
- `.bss` main 0x0218ff50-0x022900b4: 35 variables, 1,048,932 bytes, in 16 files (0 existing, 16 new, 16 of them generated); linked data 2,024,140 bytes (64.5%).

## Decisions (sturmds, 2026-10-13)

- Scope for now: `.bss` only. `.data` and `.rodata` are out of scope for the loop and the orchestrator until the `.bss` is done.
- Strings, and every other kind of data that holds game bytes, go through `inc.c` files: `make setup` extracts them from the builder's ROM, and the C includes them (the `tools/data_inc.py` pattern, `docs/workflow.md`, "Data from the ROM"). Never as literals in git (tenet 9).
- Data work stays a separate kind of job. Code agents keep `extern` for data, and nothing about data may get in the way of their work.

## Done when

The GitHub issue's "Done when" holds.
