# 0055: 15 relocations still name several overlays

- **Status:** closed
- **Area:** tools
- **Opened:** 2026-10-11
- **Milestone:** M3

## Problem

ov001, ov002, ov003, ov005 and ov006 all load at `0x02350560`. After `tools/resolve_overlay_relocs.py` (issue 0019), 15 relocations still list more than one of them, because each candidate has code or data at the target. In a shifted build the delinked object names the first candidate's symbol, which may be the wrong overlay.

| From | Target | Candidates | What is there |
|---|---|---|---|
| main `0x020dbd04` (`Palette_QueueBlockCopy`) | `0x0236d644` | 1, 6 | ov001 .data; ov006 .bss that ov006 itself points at |
| main `0x0213fe58` (.rodata) | `0x0236022e` | 1, 2 | .data in both |
| main `0x0216831c`, `0x0216a6ec` (.data) | `0x02350560` | 1, 2, 3, 5, 6 | the first function of each overlay |
| main `0x0216a7dc` (.data) | `0x02365ea8` | 1, 2, 5 | .data in ov001/ov002, middle of a function in ov005 |
| ov000 `0x022e5288` to `0x022e5c88` (.data, 10 words) | `0x023612b4` to `0x0236cfe4` | 1, 2 (and 3, 5, 6 for some) | .data in all |

## Notes

- Telling them apart needs to know which overlay is loaded when the code runs: follow the callers of the main tables to their `FS_LoadOverlay` calls, or watch it in the emulator.
- The two pointers to `0x02350560` may mean "whichever overlay is loaded" (an entry point). A symbol cannot express that; the C for those tables may need the address as a number on purpose, with a comment.
- The 10 ov000 words sit 0xa0 apart inside `data_ov000_022e022d` (an odd address, so the symbol boundaries there are guesses). They may be numbers that look like addresses, not pointers. If so, the relocations are wrong and must go: a shifted build would change those numbers.
- 2026-10-14: the stale check of `make shift` (issue 0086) reports these. Growing ov001 leaves `0x0216831c`, `0x0216a6ec` (= 0x02350560) and `0x0216a7dc` unchanged; growing ov002, ov003, ov005 or ov006 leaves the ov000 words and `Palette_QueueBlockCopy`'s `0x0236d644` unchanged. So `make shift` leaves out ov001-ov006 until this issue is done; then add them.

## Done when

Each of the 15 names one overlay, or is removed with the reason written down.

## Resolution

2026-10-14. Each of the 15 names one overlay. None was a number. The decisions are in `DECIDED` in `tools/resolve_overlay_relocs.py`:

- main `0x0213fe58`: no longer in `relocs.txt` (gone before this pass).
- main `0x0216831c`: ov002, `Battle_StartScreen` (commit 8f9ad2bd5). The attack script `data_02168400` calls `func_02001564` (unload ov001, load ov002) just before `AttackTask_StartScene`, which starts the task script `data_02168318` that holds the word.
- main `0x0216a6ec`, `0x0216a7dc`: ov005, `func_ov005_02350560` and `func_ov005_02365ea8` (same commit). Both task scripts call `func_020015f4` (unload ov001, load ov005) in their first command.
- main `0x020dbd04`: ov006, `data_ov006_0236d644`. All 16 calls to `Palette_QueueBlockCopy` are in ov006. ov006's own `Ov006_LoadSlotTiles` uses the same address as a 0x100-byte `.bss` buffer that no code writes; ov001's object there is 20 bytes, and the function copies 128 from it. The ov006 variable is no longer `static`, so main can name it.
- ov000 `0x022e5288` to `0x022e5c88` (10 words): ov001. Each is `MapHeader.script` (offset 0 of a 0xa0-byte entry of `Ov000_gMapHeaders`, maps 0xef-0xf6 and 0xff), not a number in `data_ov000_022e022d`. `MapScript_Get` returns it and the map script code reads through it in battle, where ov001 is loaded (`func_02001540` loads it back after a battle scene). The map scripts are ov001 `.data` (`docs/maps.md`): the 123 other `script`, `termsScript` and `hintScript` words of the table name ov001, and the targets here are ov001 script objects in the same ascending order.

Found on the way: ov000 `0x022e578c`, `termsScript` of map 0xf7, named ov006 (`func_ov006_02352400`): the `function` rule chose ov006 because a function starts there, but the word points into ov001's `data_ov001_023523a0`, 0x60 bytes in. That object held two terms scripts; it is split into `data_ov001_023523a0` and `data_ov001_02352400` (0x60 bytes each), and the relocation names ov001. The map files in `extracted/` keep the old name until `python3 tools/maps.py unpack` (or `make setup`) runs again; the build matches either way.

`tools/shift_build.py`'s stale check now leaves out words that have a relocation in the linked ELF: a word in main that points at ov002's first function also holds the old address of ov001's first function, and only the relocation tells them apart. With that and the decisions above, `make shift` grows every overlay and passes.
