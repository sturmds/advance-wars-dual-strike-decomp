# 0055: 15 relocations still name several overlays

- **Status:** open
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

## Done when

Each of the 15 names one overlay, or is removed with the reason written down.
