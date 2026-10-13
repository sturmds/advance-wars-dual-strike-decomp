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

## Done when

The GitHub issue's "Done when" holds.
