# 0082: Link data from C

- **Status:** in progress
- **Area:** decomp
- **Opened:** 2026-10-13
- **Milestone:** M2

## Problem

decomp.dev shows all code decompiled and linked, but 94% of the data bytes are still in dsd gap units, which have no source file. Tracked on GitHub as https://github.com/sturmds/advance-wars-dual-strike-decomp/issues/1, with the numbers and the plan (maps, then `.bss`, then `.rodata`, then `.data`).

## Notes

- Maps: the three map tables in ov000 score 100% matched data (188,932 bytes, 6.02% of all data). Tiles are one symbol with relocations into it, units one array per list in the order mwccarm emits them, and 38 false relocations dsd had found in the tables are gone. See `docs/maps.md`, "Build", and `docs/workflow.md`, "Several data objects in one file".
- `.bss`: `tools/bss.py` (`docs/bss-tool.md`). ov004 done: its 608,288 bytes link from `ov004_init3d.c` and `ov004_alloc_state.c`, and linked data is 797,220 bytes (25.4%). arm7 done: its .bss and .sbss (20,532 bytes) link from 23 existing files and 5 new ones, and linked data is 817,752 bytes (26.1%). The ARM9 modules followed (lines below); no `.bss` is left in any gap unit.
- `.bss` ov000: 93 variables, 576 bytes, in 4 files (3 existing, 1 new, generated); linked data 818,328 bytes (26.1%).
- `.bss` ov002: 43 variables, 224 bytes, in 4 files (2 existing, 2 new, 2 of them generated); linked data 818,552 bytes (26.1%).
- `.bss` ov003: 19 variables, 16,672 bytes, in 7 files (6 existing, 1 new, 1 of them generated); linked data 835,224 bytes (26.6%).
- `.bss` ov005: 13 variables, 64 bytes, in 12 files (12 existing, 0 new, 0 of them generated); linked data 835,288 bytes (26.6%).
- `.bss` ov006: 4 variables, 864 bytes, in 4 files (3 existing, 1 new, 1 of them generated); linked data 836,152 bytes (26.7%).
- `.bss` main 0x0216e020-0x0218ff50: 141 variables, 139,056 bytes, in 17 files (2 existing, 15 new, 13 of them generated); linked data 975,208 bytes (31.1%).
- `.bss` main 0x0218ff50-0x022900b4: 35 variables, 1,048,932 bytes, in 16 files (0 existing, 16 new, 16 of them generated); linked data 2,024,140 bytes (64.5%).
- `.bss` main 0x022900b4-0x0229e160: 120 variables, 57,512 bytes, in 47 files (42 existing, 5 new, 5 of them generated); linked data 2,081,656 bytes (66.4%).
- `.bss` main 0x0229e160-0x022a9870: 127 variables, 46,856 bytes, in 46 files (37 existing, 9 new, 7 of them generated); linked data 2,128,520 bytes (67.9%).
- `.bss` main 0x022a9870-0x022ad560: 141 variables, 15,452 bytes, in 56 files (53 existing, 3 new, 2 of them generated); linked data 2,144,120 bytes (68.4%).
- False relocations in main `.rodata` and `.data` and ov002 `.data`: 43 deleted after checking the code that reads each table (`docs/data-tool.md`, "Relocations to check").
- `.data` ov002 (all, 48,000 bytes, 1,989 objects, 13 existing files and 20 new) and ov001 (all, 164,608 bytes, 813 objects, 36 new files: MapScript tables, trigger lists and event scripts as words); all 1,431 ov001 relocations checked by walking the scripts from the map headers, two false ones in ov000 LZ77 streams deleted; linked data 2,356,900 bytes (75.2%).
- `.data` and `.rodata` of the small modules (`tools/data.py`): ov004 5,280 bytes in 15 files, arm7 1,932 in 18, ov003 4,564 in 15, ov005 15,456 in 55, ov006 13,408 in 33, dtcm 4,832 in 12; 26 aliases listed, 4 false relocations deleted and one narrowed to ov001; linked data 2,495,652 bytes (79.6%), counting the main, ov001 and ov002 data that landed before it.
- `.rodata` main 0x021298b8-0x02151c68: 249 objects, 164,784 bytes, in 82 files (19 existing, 63 new), five commits by address range; `gSineTable` is one s16[320] table (sine plus 64 entries for the cosine), `gGxTexLcdcPages0` one u16[16][3] table, with the other names as aliases; one false relocation (ov000 0x022c976c, inside a sprite cell offset table) and nine unused symbols deleted. Left: the MSL exception table at 0x02129760 (0x158 bytes), which the linker builds from the objects' exception records.
- Exception table, main 0x02129760-0x021298b8 (344 bytes): built by the linker from the records of 28 functions in the files `config/AWRE/exception_tables.txt` lists, which build with `-Cpp_exceptions on` while every other file builds with it off (issue 0001 closed; `docs/data-tool.md`, "The exception table"). Data linked 98.38% to 98.39% (3,085,444 to 3,085,788 bytes).
- `.data` and `.rodata` ov000 (all, 453,008 bytes in 5,796 objects, about 250 new files and 9 existing ones, seven commits by address range): `Ov000_gUnitStats` and `Ov000_gForceRankTable` written as their structs (a union is written as its first member), the terrain table at 0x022f4cb8 and most unnamed tables as words; 42 aliases listed as real fields after checking their code; 7 false relocations inside LZ77 streams deleted (12 more were already gone on mainline) with 14 unused symbols; linked data 3,085,444 bytes (98.4%), counting the data that landed before it.
- `.data` main 0x02151c80-0x0216e020 (all): 2,180 objects, 115,616 bytes, in 251 files (60 existing, 191 new), five commits by address range. `gCoTable` is CoRecord[CO_COUNT] (data.py now writes a union as its first member), the stat rows and tag records it points at are CoStatRow and CoTagRecord, `gMapPaletteFiles` is WeatherFiles[4]. 78 aliases listed as real fields; three unused symbols deleted (their relocations were false ones dropped before); three task script pointers narrowed to ov002 or ov005; linked data 3,135,780 bytes (99.99%), counting the work of the other agents that landed before it.

## Decisions (sturmds, 2026-10-13)

- Scope for now: `.bss` only. `.data` and `.rodata` are out of scope for the loop and the orchestrator until the `.bss` is done.
- Strings, and every other kind of data that holds game bytes, go through `inc.c` files: `make setup` extracts them from the builder's ROM, and the C includes them (the `tools/data_inc.py` pattern, `docs/workflow.md`, "Data from the ROM"). Never as literals in git (tenet 9).
- Data work stays a separate kind of job. Code agents keep `extern` for data, and nothing about data may get in the way of their work.

## Done when

The GitHub issue's "Done when" holds.
