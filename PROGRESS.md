# Decompilation progress

Written by `tools/progress.py --write PROGRESS.md` from `hooks/pre-commit`; do not edit.
A function counts as done when its file is marked `complete` in `delinks.txt`, so the
build links it from `src/`. Functions written as `asm` inside a .c file count separately.

## Totals

ARM9 is main, ITCM and the overlays; ARM7 is `arm7.bin`. `game`, `SDK/library` and `all`
add both CPUs.

| Code | Functions in C | Bytes in C | % of bytes | Functions in asm | Bytes in asm |
|---|---:|---:|---:|---:|---:|
| ARM9 game | 6682 / 6700 | 1537696 / 1553840 | 98.96% | 0 | 0 |
| ARM9 SDK/library | 645 / 833 | 104112 / 135226 | 76.99% | 176 | 20718 |
| ARM9 all | 7327 / 7533 | 1641808 / 1689066 | 97.20% | 176 | 20718 |
| ARM7 SDK/library | 949 / 1020 | 148704 / 154300 | 96.37% | 69 | 4960 |
| ARM7 all | 949 / 1020 | 148704 / 154300 | 96.37% | 69 | 4960 |
| game | 6682 / 6700 | 1537696 / 1553840 | 98.96% | 0 | 0 |
| SDK/library | 1594 / 1853 | 252816 / 289526 | 87.32% | 245 | 25678 |
| all | 8276 / 8553 | 1790512 / 1843366 | 97.13% | 245 | 25678 |

## By module

Rows with nothing done are left out, but count in the totals; ARM7's row is always shown.

| Module | Code | Functions in C | Bytes in C | % of bytes | Functions in asm |
|---|---|---:|---:|---:|---:|
| main | game | 5171 / 5180 | 1075412 / 1082632 | 99.33% | 0 |
| main | SDK/library | 638 / 822 | 103200 / 133690 | 77.19% | 172 |
| itcm | game | 102 / 102 | 21896 / 21896 | 100.00% | 0 |
| itcm | SDK/library | 7 / 11 | 912 / 1536 | 59.38% | 4 |
| ov000 | game | 119 / 120 | 25000 / 25300 | 98.81% | 0 |
| ov001 | game | 76 / 76 | 7472 / 7472 | 100.00% | 0 |
| ov002 | game | 161 / 164 | 58840 / 64208 | 91.64% | 0 |
| ov003 | game | 234 / 235 | 81996 / 82696 | 99.15% | 0 |
| ov004 | game | 232 / 236 | 64808 / 67364 | 96.21% | 0 |
| ov005 | game | 266 / 266 | 96684 / 96684 | 100.00% | 0 |
| ov006 | game | 321 / 321 | 105588 / 105588 | 100.00% | 0 |
| arm7 | SDK/library | 949 / 1020 | 148704 / 154300 | 96.37% | 69 |

Modules with nothing done: none.

## Partial functions

Functions in C files that the build does not use yet (not marked `complete`), because
they do not match. Each one is compiled and compared with the original by `fn.py diff`:
"Differ" counts the rows of its aligned listing that differ (an instruction only one side
has counts as one row; a relocation naming another symbol at the same address does not
count). "Match" is objdiff's percentage. The list ranks by "Differ", then "Match":
objdiff's score charges one wrong opcode as much as 12 wrong registers (issue 0067).

| Code | Functions | Bytes | Rows that match |
|---|---:|---:|---:|
| ARM9 game | 18 | 16144 | 3793 / 4052 (93.61%) |
| ARM9 SDK/library | 12 | 10396 | 1896 / 2683 (70.67%) |
| ARM7 SDK/library | 2 | 636 | 148 / 159 (93.08%) |
| all | 32 | 27176 | 5837 / 6894 (84.67%) |

| Rows that differ | Functions | Bytes |
|---|---:|---:|
| 1 to 2 | 0 | 0 |
| 3 to 10 | 11 | 9924 |
| 11 to 50 | 18 | 9236 |
| more than 50 | 3 | 8016 |

Closest first.

| File | Function | Bytes | Differ | Match |
|---|---|---:|---:|---:|
| `src/game/main/ai/ai_unit_upgrade_wait_action.c` | AiUnit_UpgradeWaitAction | 3924 | 3 | 99.79% |
| `src/arm7/sdk/wm/func_arm7_027c6020.c` | func_arm7_027c6020 | 152 | 3 | 99.47% |
| `src/sdk/card/cardi_set_backup_spec.c` | func_02115858 | 472 | 3 | 97.80% |
| `src/game/main/menus/func_0204e808.c` | func_0204e808 | 248 | 4 | 95.63% |
| `src/game/main/map/func_020cb848.c` | func_020cb848 | 428 | 5 | 97.38% |
| `src/game/main/menus/func_0204f160.c` | func_0204f160 | 160 | 6 | 94.00% |
| `src/game/main/menus/func_0204f200.c` | func_0204f200 | 160 | 6 | 94.00% |
| `src/game/ov002/func_ov002_023549c8.c` | func_ov002_023549c8 | 2312 | 8 | 99.92% |
| `src/arm7/sdk/wm/func_arm7_027c632c.c` | func_arm7_027c632c | 484 | 8 | 99.63% |
| `src/game/main/units/move_path_follow_cursor.c` | MovePath_FollowCursor | 916 | 10 | 99.74% |
| `src/game/main/link/func_0207f550.c` | func_0207f550 | 668 | 10 | 99.64% |
| `src/game/ov002/func_ov002_02352754.c` | func_ov002_02352754 | 2564 | 11 | 99.91% |
| `src/game/ov003/func_ov003_02361c7c.c` | func_ov003_02361c7c | 700 | 13 | 99.46% |
| `src/game/ov002/battle_fx_start_a.c` | func_ov002_02357e5c | 492 | 14 | 95.83% |
| `src/game/ov000/unit_info_panel/unit_info_panel_draw_page.c` | UnitInfoPanel_DrawPage | 300 | 14 | 89.08% |
| `src/game/ov004/func_ov004_022bacd8.c` | func_ov004_022bacd8 | 212 | 15 | 94.13% |
| `src/game/main/menus/func_0204f0c0.c` | func_0204f0c0 | 160 | 17 | 85.97% |
| `src/sdk/msl/__fp_trap_f.c` | __fp_trap_f | 72 | 17 | 52.39% |
| `src/sdk/msl/__fp_trap_ff.c` | __fp_trap_ff | 76 | 18 | 54.63% |
| `src/sdk/msl/func_02121ea8.c` | func_02121ea8 | 76 | 18 | 54.63% |
| `src/sdk/msl/__fp_trap_d.c` | __fp_trap_d | 80 | 19 | 56.65% |
| `src/sdk/msl/func_02121d58.c` | func_02121d58 | 88 | 21 | 60.14% |
| `src/sdk/msl/__fp_trap_dd.c` | __fp_trap_dd | 92 | 22 | 61.65% |
| `src/game/ov004/func_ov004_022ad71c.c` | func_ov004_022ad71c | 352 | 25 | 94.72% |
| `src/game/ov004/func_ov004_022b1f98.c` | func_ov004_022b1f98 | 1736 | 26 | 99.70% |
| `src/game/ov004/ov004_blit_block_with_flags.c` | Ov004_BlitBlockWithFlags | 256 | 30 | 90.39% |
| `src/sdk/msl/__fp_trap.c` | __fp_trap | 256 | 30 | 84.98% |
| `src/game/main/menus/combat_map_draw_preview_block.c` | CombatMap_DrawPreviewBlock | 556 | 42 | 98.06% |
| `src/sdk/isd/isd_double_divide_by_ten.c` | func_0211fe0c | 1168 | 44 | 96.78% |
| `src/sdk/msl/double2hex.c` | double2hex | 948 | 121 | 86.82% |
| `src/sdk/isd/isd_v_format.c` | Isd_VFormat | 4960 | 190 | 99.31% |
| `src/sdk/msl/__pformatter.c` | __pformatter | 2108 | 284 | 88.31% |
