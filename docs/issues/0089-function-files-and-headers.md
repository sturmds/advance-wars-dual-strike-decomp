# 0089: One file per function, and prototypes copied into each caller

- **Status:** in progress
- **Area:** decomp
- **Opened:** 2026-10-14
- **Milestone:** M3

## Problem

Each function was decompiled into its own C file, and each caller declares the functions it calls at its own top. `Player_IsInGame` is declared in about 60 files. On 2026-10-14 `src/` held about 18,900 such prototypes in 5,200 of its 9,142 C files, against about 880 function prototypes in `include/`. The compiler never sees a caller's prototype next to the definition, so the two can disagree (`tools/protocheck.py`, issue 0062), and an editor cannot go from a call to the one declaration that belongs to the definition.

## Notes

`tools/merge_files.py` merges a run of neighbouring one-function files into one file, writes a header with a prototype per function and replaces the prototypes in every caller with an include of it (`docs/workflow.md`, "Merging files").

What the pilot found (2026-10-14, four groups of AI functions):

- mwccarm 2.0-72 emits a file's functions last to first, and has no `-ipa file` to change that. A merged file holds its functions in reverse address order, as `src/game/main/func_02026868.c` already did. The original source files were written that way round too.
- The functions in one file are not inlined into each other: `AiTurn_Update` calls `AiTurn_Restart`, defined above it, and its code did not change.
- `.bss` variables of the merged files are reordered by mwcc's heapsort. The tool writes their definitions in the order `tools/mwcc_order.py` gives for their addresses. When no order gives the addresses, the files did not come from one original file, and the tool stops.
- Two prototypes of one callee that differ in the merged files are a conflict. The tool keeps the one that matches the definition. `func_020f5888` is defined with `int` arguments; `AiShowAction_Action22` matched only with an `s16` prototype, and matches with the definition's prototype and `(s16)` casts in its call. `s16` everywhere broke `UnitDrop_FindNextCity`, whose arguments are `int` fields.
- A file that was a caller of an earlier group loses its prototypes; the tool removes it with `git rm -f`, after reading its current text.

The method is in `docs/readability.md` (tenet 11). Each step left every function's code as it was (`tools/objcmp.py compare`) and the ROM matching.

Done on 2026-10-14 and 2026-10-15:

- `data_027c027c` and `data_027c0284` are declared once, in `game/Unk_02080440.h` (1,633 and 1,360 copies removed). `data_027c0280` and `data_027c0288` are in `game/Unk_02088d04.h`, `gMenuState` in `game/menu_state.h`.
- `MAP_STATE` replaces `STATE` and `GAME` in 1,081 files; `MAP_TILES`, `MAP_TILE_INDEX` and `MAP_TILE_AT` replace `MAP`, `TILE_INDEX` and `TILE` where they mean the same. Kept: the forms written for a match (`rowStart[y] + x` changes the code of 10 functions; the pointer sums with `sizeof`), the ov003 and ov006 `STATE` and `MAP`, the range map's `TILE`.
- New topic headers: `game/math.h`, `game/screen.h` (functions of `src/game/main/screen/` that had none), and `Oam_SetAffineParams` in `game/oam_buffer.h`.
- `src/game/main/ai/` went from 440 files to 75: 35 topic files with a header each in `include/game/ai/`, the single functions between non-AI files declared in `game/ai.h`, and three functions in their own files (two with a `#pragma`, `func_02098e7c` and `AiEmptyTransport_GoToFootUnit` with their own prototypes of a callee).
- Prototypes found by `decls.py unify`: `Ai_CanEndMoveAt`, `Ai_HasEnemyWithin`, `Ai_HasVisibleEnemyWithin`, `AiUnit_HasFuelForTile`, `Unit_IsEnemyTile` take `s16` coordinates; `Unit_IsCurrentPlayers` an `s16` slot; `UnitType_GetAttackKind`, `AiBuild_GetTargetGroup`, `AiProfile_AllowsBuild`, `AiBuild_RateAttackerVsTarget` a `u8` unit type; `AiUnit_PerformAction` is `(s16, s16, u8, u8, u8)`; `AiUnit_AdjustAction` takes `s16` coordinates.
- Local copies of a header's declaration: 6,176 to 0. `config/AWRE/local_decls.txt` lists the ones that stay: 16 needed to match, about 260 that do not compile with the header's type yet, and about 100 in files whose include would clash with one of those. `make release` fails on any other (`decls.py check`).

Left:

- About 16,700 file-scope prototypes in 4,800 C files are of functions no header declares yet. Give them homes directory by directory (`decls.py declare game/DIR.h --dir src/game/main/DIR`) or by topic, then `dedupe`.
- The entries of `local_decls.txt` that do not compile with the header's type: mostly pointer types (`void *` against a struct pointer); fix the callers' types, then `reconcile`.
- Merge the other directories by topic as `ai/` was.

## Done when

- Every file in `src/game/main/ai/` holds a group of functions with a header in `include/game/ai/`, and no C file in `src/` declares a function that a header declares.
- `make release` fails on a new file-scope prototype of a function that a header declares.
