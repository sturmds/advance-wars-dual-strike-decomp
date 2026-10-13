# Decompilation progress

Written by `tools/progress.py --write PROGRESS.md` from `hooks/pre-commit`; do not edit.
A function counts as done when its file is marked `complete` in `delinks.txt`, so the
build links it from `src/`. Functions written as `asm` inside a .c file count separately.

## Totals

ARM9 is main, ITCM and the overlays; ARM7 is `arm7.bin`. `game`, `SDK/library` and `all`
add both CPUs.

| Code | Functions in C | Bytes in C | % of bytes | Functions in asm | Bytes in asm |
|---|---:|---:|---:|---:|---:|
| ARM9 game | 6698 / 6700 | 1552984 / 1553840 | 99.94% | 0 | 0 |
| ARM9 SDK/library | 657 / 833 | 114512 / 135230 | 84.68% | 176 | 20718 |
| ARM9 all | 7355 / 7533 | 1667496 / 1689070 | 98.72% | 176 | 20718 |
| ARM7 SDK/library | 950 / 1020 | 149188 / 154300 | 96.69% | 69 | 4960 |
| ARM7 all | 950 / 1020 | 149188 / 154300 | 96.69% | 69 | 4960 |
| game | 6698 / 6700 | 1552984 / 1553840 | 99.94% | 0 | 0 |
| SDK/library | 1607 / 1853 | 263700 / 289530 | 91.08% | 245 | 25678 |
| all | 8305 / 8553 | 1816684 / 1843370 | 98.55% | 245 | 25678 |

## By module

Rows with nothing done are left out, but count in the totals; ARM7's row is always shown.

| Module | Code | Functions in C | Bytes in C | % of bytes | Functions in asm |
|---|---|---:|---:|---:|---:|
| main | game | 5179 / 5180 | 1082076 / 1082632 | 99.95% | 0 |
| main | SDK/library | 650 / 822 | 113600 / 133694 | 84.97% | 172 |
| itcm | game | 102 / 102 | 21896 / 21896 | 100.00% | 0 |
| itcm | SDK/library | 7 / 11 | 912 / 1536 | 59.38% | 4 |
| ov000 | game | 119 / 120 | 25000 / 25300 | 98.81% | 0 |
| ov001 | game | 76 / 76 | 7472 / 7472 | 100.00% | 0 |
| ov002 | game | 164 / 164 | 64208 / 64208 | 100.00% | 0 |
| ov003 | game | 235 / 235 | 82696 / 82696 | 100.00% | 0 |
| ov004 | game | 236 / 236 | 67364 / 67364 | 100.00% | 0 |
| ov005 | game | 266 / 266 | 96684 / 96684 | 100.00% | 0 |
| ov006 | game | 321 / 321 | 105588 / 105588 | 100.00% | 0 |
| arm7 | SDK/library | 950 / 1020 | 149188 / 154300 | 96.69% | 69 |

Modules with nothing done: none.

## decomp.dev report

From `build/artifacts/0.1_report/report.json`, which `make release` writes (`tools/decomp_dev_report.py`).
decomp.dev shows "Decompiled" (`matched_code`) and "Fully linked" (`complete_code`).
Functions written as `asm` count as neither. "Code" also holds code in dsd gap units that
has no function, so it is a little larger than the totals above (issue 0071).

| Module | Code | Decompiled | Fully linked | Functions matched |
|---|---:|---:|---:|---:|
| main | 1216386 | 97.89% | 98.30% | 5828 / 6004 |
| itcm | 24088 | 94.69% | 94.69% | 109 / 115 |
| ov000 | 25300 | 98.81% | 98.81% | 119 / 120 |
| ov001 | 7472 | 100.00% | 100.00% | 76 / 76 |
| ov002 | 64208 | 100.00% | 100.00% | 164 / 164 |
| ov003 | 82696 | 100.00% | 100.00% | 235 / 235 |
| ov004 | 67364 | 100.00% | 100.00% | 236 / 236 |
| ov005 | 96684 | 100.00% | 100.00% | 266 / 266 |
| ov006 | 105588 | 100.00% | 100.00% | 321 / 321 |
| arm7 | 155236 | 96.10% | 96.10% | 950 / 1038 |
| all | 1845022 | 98.20% | 98.46% | 8304 / 8575 |

## Partial functions

Functions in C files that the build does not use yet (not marked `complete`), because
they do not match. Each one is compiled and compared with the original by `fn.py diff`:
"Differ" counts the rows of its aligned listing that differ (an instruction only one side
has counts as one row; a relocation naming another symbol at the same address does not
count). "Match" is objdiff's percentage. The list ranks by "Differ", then "Match":
objdiff's score charges one wrong opcode as much as 12 wrong registers (issue 0067).

| Code | Functions | Bytes | Rows that match |
|---|---:|---:|---:|
| ARM9 game | 2 | 856 | 161 / 217 (74.19%) |
| ARM7 SDK/library | 1 | 152 | 36 / 38 (94.74%) |
| all | 3 | 1008 | 197 / 255 (77.25%) |

| Rows that differ | Functions | Bytes |
|---|---:|---:|
| 1 to 2 | 1 | 152 |
| 3 to 10 | 0 | 0 |
| 11 to 50 | 2 | 856 |
| more than 50 | 0 | 0 |

Closest first.

| File | Function | Bytes | Differ | Match |
|---|---|---:|---:|---:|
| `src/arm7/sdk/wm/func_arm7_027c6020.c` | func_arm7_027c6020 | 152 | 2 | 98.39% |
| `src/game/ov000/unit_info_panel/unit_info_panel_draw_page.c` | UnitInfoPanel_DrawPage | 300 | 14 | 89.08% |
| `src/game/main/menus/combat_map_draw_preview_block.c` | CombatMap_DrawPreviewBlock | 556 | 42 | 98.06% |
