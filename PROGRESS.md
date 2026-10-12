# Decompilation progress

Written by `tools/progress.py --write PROGRESS.md` from `hooks/pre-commit`; do not edit.
A function counts as done when its file is marked `complete` in `delinks.txt`, so the
build links it from `src/`. Functions written as `asm` inside a .c file count separately.

## Totals

ARM9 is main, ITCM and the overlays; ARM7 is `arm7.bin`. `game`, `SDK/library` and `all`
add both CPUs.

| Code | Functions in C | Bytes in C | % of bytes | Functions in asm | Bytes in asm |
|---|---:|---:|---:|---:|---:|
| ARM9 game | 6665 / 6700 | 1514096 / 1553840 | 97.44% | 0 | 0 |
| ARM9 SDK/library | 642 / 833 | 102952 / 135226 | 76.13% | 176 | 20718 |
| ARM9 all | 7307 / 7533 | 1617048 / 1689066 | 95.74% | 176 | 20718 |
| ARM7 SDK/library | 948 / 1020 | 147744 / 154300 | 95.75% | 69 | 4960 |
| ARM7 all | 948 / 1020 | 147744 / 154300 | 95.75% | 69 | 4960 |
| game | 6665 / 6700 | 1514096 / 1553840 | 97.44% | 0 | 0 |
| SDK/library | 1590 / 1853 | 250696 / 289526 | 86.59% | 245 | 25678 |
| all | 8255 / 8553 | 1764792 / 1843366 | 95.74% | 245 | 25678 |

## By module

Rows with nothing done are left out, but count in the totals; ARM7's row is always shown.

| Module | Code | Functions in C | Bytes in C | % of bytes | Functions in asm |
|---|---|---:|---:|---:|---:|
| main | game | 5159 / 5180 | 1055196 / 1082632 | 97.47% | 0 |
| main | SDK/library | 635 / 822 | 102040 / 133690 | 76.33% | 172 |
| itcm | game | 102 / 102 | 21896 / 21896 | 100.00% | 0 |
| itcm | SDK/library | 7 / 11 | 912 / 1536 | 59.38% | 4 |
| ov000 | game | 119 / 120 | 25000 / 25300 | 98.81% | 0 |
| ov001 | game | 76 / 76 | 7472 / 7472 | 100.00% | 0 |
| ov002 | game | 159 / 164 | 56424 / 64208 | 87.88% | 0 |
| ov003 | game | 234 / 235 | 81996 / 82696 | 99.15% | 0 |
| ov004 | game | 232 / 236 | 64808 / 67364 | 96.21% | 0 |
| ov005 | game | 264 / 266 | 96316 / 96684 | 99.62% | 0 |
| ov006 | game | 320 / 321 | 104988 / 105588 | 99.43% | 0 |
| arm7 | SDK/library | 948 / 1020 | 147744 / 154300 | 95.75% | 69 |

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
| ARM9 game | 35 | 39744 | 9456 / 9967 (94.87%) |
| ARM9 SDK/library | 15 | 11556 | 2138 / 2975 (71.87%) |
| ARM7 SDK/library | 3 | 1596 | 323 / 399 (80.95%) |
| all | 53 | 52896 | 11917 / 13341 (89.33%) |

| Rows that differ | Functions | Bytes |
|---|---:|---:|
| 1 to 2 | 3 | 4872 |
| 3 to 10 | 18 | 11156 |
| 11 to 50 | 28 | 27892 |
| more than 50 | 4 | 8976 |

Closest first.

| File | Function | Bytes | Differ | Match |
|---|---|---:|---:|---:|
| `src/game/main/menus/campaign_map_init.c` | CampaignMap_Init | 3036 | 1 | 99.91% |
| `src/game/main/ai/ai_site_group_rate_surroundings.c` | AiSiteGroup_RateSurroundings | 1584 | 2 | 99.99% |
| `src/sdk/card/func_021154e8.c` | func_021154e8 | 252 | 2 | 96.83% |
| `src/arm7/sdk/wm/func_arm7_027c6020.c` | func_arm7_027c6020 | 152 | 3 | 99.47% |
| `src/sdk/card/cardi_set_backup_spec.c` | func_02115858 | 472 | 3 | 97.80% |
| `src/game/main/func_02058b1c.c` | func_02058b1c | 1000 | 4 | 99.89% |
| `src/game/main/ai/func_02099ba0.c` | func_02099ba0 | 524 | 4 | 99.77% |
| `src/game/main/menus/func_0204e808.c` | func_0204e808 | 248 | 4 | 95.63% |
| `src/sdk/card/cardi_start_rom_dma.c` | func_021162d8 | 340 | 5 | 99.71% |
| `src/game/ov002/func_ov002_023552d0.c` | func_ov002_023552d0 | 1044 | 5 | 98.72% |
| `src/game/main/map/func_020cb848.c` | func_020cb848 | 428 | 5 | 97.38% |
| `src/game/main/map/func_020c0aa8.c` | func_020c0aa8 | 916 | 6 | 99.52% |
| `src/game/ov005/func_ov005_0235b4c0.c` | func_ov005_0235b4c0 | 180 | 6 | 98.98% |
| `src/game/main/menus/func_0204f160.c` | func_0204f160 | 160 | 6 | 94.00% |
| `src/game/main/menus/func_0204f200.c` | func_0204f200 | 160 | 6 | 94.00% |
| `src/game/main/screen/screen_prepare_h_blank_regs.c` | Screen_PrepareHBlankRegs | 1656 | 7 | 99.90% |
| `src/game/main/ai/ai_oozium_move_to_target.c` | AiOozium_MoveToTarget | 1568 | 7 | 99.90% |
| `src/game/main/map/func_020f9450.c` | func_020f9450 | 240 | 7 | 94.42% |
| `src/arm7/sdk/wm/func_arm7_027c632c.c` | func_arm7_027c632c | 484 | 8 | 99.63% |
| `src/game/main/units/move_path_follow_cursor.c` | MovePath_FollowCursor | 916 | 10 | 99.74% |
| `src/game/main/link/func_0207f550.c` | func_0207f550 | 668 | 10 | 99.64% |
| `src/game/ov002/func_ov002_02352754.c` | func_ov002_02352754 | 2564 | 11 | 99.91% |
| `src/game/main/graphics/quad/quad_sprite_draw.c` | AnimSpriteTask_Draw | 484 | 11 | 94.12% |
| `src/game/ov002/battle_fx_start_a.c` | func_ov002_02357e5c | 492 | 14 | 95.83% |
| `src/game/ov000/unit_info_panel/unit_info_panel_draw_page.c` | UnitInfoPanel_DrawPage | 300 | 14 | 89.08% |
| `src/game/main/ai/ai_unit_upgrade_wait_action.c` | AiUnit_UpgradeWaitAction | 3924 | 15 | 99.98% |
| `src/game/ov003/func_ov003_02361c7c.c` | func_ov003_02361c7c | 700 | 15 | 97.22% |
| `src/game/ov004/func_ov004_022bacd8.c` | func_ov004_022bacd8 | 212 | 15 | 94.13% |
| `src/sdk/pm/pm_go_sleep_mode.c` | func_02114ae8 | 568 | 16 | 97.27% |
| `src/game/main/menus/func_0204f0c0.c` | func_0204f0c0 | 160 | 17 | 85.97% |
| `src/sdk/msl/__fp_trap_f.c` | __fp_trap_f | 72 | 17 | 52.39% |
| `src/sdk/msl/__fp_trap_ff.c` | __fp_trap_ff | 76 | 18 | 54.63% |
| `src/sdk/msl/func_02121ea8.c` | func_02121ea8 | 76 | 18 | 54.63% |
| `src/sdk/msl/__fp_trap_d.c` | __fp_trap_d | 80 | 19 | 56.65% |
| `src/game/ov002/func_ov002_023549c8.c` | func_ov002_023549c8 | 2312 | 20 | 99.38% |
| `src/sdk/msl/func_02121d58.c` | func_02121d58 | 88 | 21 | 60.14% |
| `src/game/main/screen/screen_apply_main.c` | Screen_ApplyMain | 3848 | 22 | 99.82% |
| `src/game/main/screen/screen_apply_sub.c` | Screen_ApplySub | 3624 | 22 | 99.81% |
| `src/sdk/msl/__fp_trap_dd.c` | __fp_trap_dd | 92 | 22 | 61.65% |
| `src/game/ov004/func_ov004_022ad71c.c` | func_ov004_022ad71c | 352 | 25 | 94.72% |
| `src/game/ov002/func_ov002_02358c40.c` | func_ov002_02358c40 | 1372 | 26 | 99.73% |
| `src/game/ov004/func_ov004_022b1f98.c` | func_ov004_022b1f98 | 1736 | 26 | 99.70% |
| `src/game/main/ai/ai_build_rate_site_danger.c` | AiBuild_RateSiteDanger | 1736 | 26 | 97.49% |
| `src/game/ov006/func_ov006_023557b8.c` | func_ov006_023557b8 | 600 | 28 | 99.07% |
| `src/game/ov004/ov004_blit_block_with_flags.c` | Ov004_BlitBlockWithFlags | 256 | 30 | 90.39% |
| `src/sdk/msl/__fp_trap.c` | __fp_trap | 256 | 30 | 84.98% |
| `src/game/main/menus/combat_map_draw_preview_block.c` | CombatMap_DrawPreviewBlock | 556 | 42 | 98.06% |
| `src/game/ov005/func_ov005_0235b5dc.c` | func_ov005_0235b5dc | 188 | 42 | 72.36% |
| `src/sdk/isd/isd_double_divide_by_ten.c` | func_0211fe0c | 1168 | 44 | 96.78% |
| `src/arm7/sdk/wm/wmsp_start_scan_ex.c` | func_arm7_027c342c | 960 | 65 | 98.44% |
| `src/sdk/msl/double2hex.c` | double2hex | 948 | 121 | 86.82% |
| `src/sdk/isd/isd_v_format.c` | Isd_VFormat | 4960 | 217 | 99.28% |
| `src/sdk/msl/__pformatter.c` | __pformatter | 2108 | 284 | 88.31% |
