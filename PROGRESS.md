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
| ARM7 SDK/library | 947 / 1020 | 147644 / 154300 | 95.69% | 69 | 4960 |
| ARM7 all | 947 / 1020 | 147644 / 154300 | 95.69% | 69 | 4960 |
| game | 6665 / 6700 | 1514096 / 1553840 | 97.44% | 0 | 0 |
| SDK/library | 1589 / 1853 | 250596 / 289526 | 86.55% | 245 | 25678 |
| all | 8254 / 8553 | 1764692 / 1843366 | 95.73% | 245 | 25678 |

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
| arm7 | SDK/library | 947 / 1020 | 147644 / 154300 | 95.69% | 69 |

Modules with nothing done: none.

## Partial functions

Functions in C files that the build does not use yet (not marked `complete`), because
they do not match. Each one is compiled and compared with the original by `fn.py diff`:
"Match" is objdiff's percentage, "Differ" the rows of its aligned listing that differ
(an instruction only one side has counts as one row).

| Code | Functions | Bytes | Rows that match |
|---|---:|---:|---:|
| ARM9 game | 35 | 39744 | 9455 / 9967 (94.86%) |
| ARM9 SDK/library | 15 | 11556 | 2138 / 2975 (71.87%) |
| ARM7 SDK/library | 4 | 1696 | 335 / 424 (79.01%) |
| all | 54 | 52996 | 11928 / 13366 (89.24%) |

| Rows that differ | Functions | Bytes |
|---|---:|---:|
| 1 to 2 | 3 | 4872 |
| 3 to 10 | 18 | 11156 |
| 11 to 50 | 29 | 27992 |
| more than 50 | 4 | 8976 |

Closest first.

| File | Function | Bytes | Match | Differ |
|---|---|---:|---:|---:|
| `src/game/main/ai/ai_site_group_rate_surroundings.c` | AiSiteGroup_RateSurroundings | 1584 | 99.99% | 2 |
| `src/game/main/menus/campaign_map_init.c` | CampaignMap_Init | 3036 | 99.83% | 2 |
| `src/sdk/card/func_021154e8.c` | func_021154e8 | 252 | 96.83% | 2 |
| `src/arm7/sdk/wm/func_arm7_027c6020.c` | func_arm7_027c6020 | 152 | 99.47% | 3 |
| `src/sdk/card/cardi_set_backup_spec.c` | func_02115858 | 472 | 97.80% | 3 |
| `src/game/main/func_02058b1c.c` | func_02058b1c | 1000 | 99.89% | 4 |
| `src/game/main/ai/func_02099ba0.c` | func_02099ba0 | 524 | 99.77% | 4 |
| `src/game/main/menus/func_0204e808.c` | func_0204e808 | 248 | 95.63% | 4 |
| `src/sdk/card/cardi_start_rom_dma.c` | func_021162d8 | 340 | 99.71% | 5 |
| `src/game/ov002/func_ov002_023552d0.c` | func_ov002_023552d0 | 1044 | 98.72% | 5 |
| `src/game/main/map/func_020cb848.c` | func_020cb848 | 428 | 97.38% | 5 |
| `src/game/main/map/func_020c0aa8.c` | func_020c0aa8 | 916 | 99.52% | 6 |
| `src/game/ov005/func_ov005_0235b4c0.c` | func_ov005_0235b4c0 | 180 | 98.98% | 6 |
| `src/game/main/menus/func_0204f160.c` | func_0204f160 | 160 | 94.00% | 6 |
| `src/game/main/menus/func_0204f200.c` | func_0204f200 | 160 | 94.00% | 6 |
| `src/game/main/screen/screen_prepare_h_blank_regs.c` | Screen_PrepareHBlankRegs | 1656 | 99.90% | 7 |
| `src/game/main/ai/ai_oozium_move_to_target.c` | AiOozium_MoveToTarget | 1568 | 99.90% | 7 |
| `src/game/main/map/func_020f9450.c` | func_020f9450 | 240 | 94.42% | 7 |
| `src/arm7/sdk/wm/func_arm7_027c632c.c` | func_arm7_027c632c | 484 | 99.63% | 8 |
| `src/game/main/units/move_path_follow_cursor.c` | MovePath_FollowCursor | 916 | 99.74% | 10 |
| `src/game/main/link/func_0207f550.c` | func_0207f550 | 668 | 99.64% | 10 |
| `src/game/ov002/func_ov002_02352754.c` | func_ov002_02352754 | 2564 | 99.91% | 11 |
| `src/game/main/graphics/quad/quad_sprite_draw.c` | AnimSpriteTask_Draw | 484 | 94.12% | 11 |
| `src/arm7/sdk/wl/func_arm7_027cab1c.c` | func_arm7_027cab1c | 100 | 95.60% | 13 |
| `src/game/ov002/battle_fx_start_a.c` | func_ov002_02357e5c | 492 | 95.83% | 14 |
| `src/game/ov000/unit_info_panel/unit_info_panel_draw_page.c` | UnitInfoPanel_DrawPage | 300 | 89.08% | 14 |
| `src/game/main/ai/ai_unit_upgrade_wait_action.c` | AiUnit_UpgradeWaitAction | 3924 | 99.98% | 15 |
| `src/game/ov003/func_ov003_02361c7c.c` | func_ov003_02361c7c | 700 | 97.22% | 15 |
| `src/game/ov004/func_ov004_022bacd8.c` | func_ov004_022bacd8 | 212 | 94.13% | 15 |
| `src/sdk/pm/pm_go_sleep_mode.c` | func_02114ae8 | 568 | 97.27% | 16 |
| `src/game/main/menus/func_0204f0c0.c` | func_0204f0c0 | 160 | 85.97% | 17 |
| `src/sdk/msl/__fp_trap_f.c` | __fp_trap_f | 72 | 52.39% | 17 |
| `src/sdk/msl/__fp_trap_ff.c` | __fp_trap_ff | 76 | 54.63% | 18 |
| `src/sdk/msl/func_02121ea8.c` | func_02121ea8 | 76 | 54.63% | 18 |
| `src/sdk/msl/__fp_trap_d.c` | __fp_trap_d | 80 | 56.65% | 19 |
| `src/game/ov002/func_ov002_023549c8.c` | func_ov002_023549c8 | 2312 | 99.38% | 20 |
| `src/sdk/msl/func_02121d58.c` | func_02121d58 | 88 | 60.14% | 21 |
| `src/game/main/screen/screen_apply_main.c` | Screen_ApplyMain | 3848 | 99.82% | 22 |
| `src/game/main/screen/screen_apply_sub.c` | Screen_ApplySub | 3624 | 99.81% | 22 |
| `src/sdk/msl/__fp_trap_dd.c` | __fp_trap_dd | 92 | 61.65% | 22 |
| `src/game/ov004/func_ov004_022ad71c.c` | func_ov004_022ad71c | 352 | 94.72% | 25 |
| `src/game/ov002/func_ov002_02358c40.c` | func_ov002_02358c40 | 1372 | 99.73% | 26 |
| `src/game/ov004/func_ov004_022b1f98.c` | func_ov004_022b1f98 | 1736 | 99.70% | 26 |
| `src/game/main/ai/ai_build_rate_site_danger.c` | AiBuild_RateSiteDanger | 1736 | 97.49% | 26 |
| `src/game/ov006/func_ov006_023557b8.c` | func_ov006_023557b8 | 600 | 99.07% | 28 |
| `src/game/ov004/ov004_blit_block_with_flags.c` | Ov004_BlitBlockWithFlags | 256 | 90.39% | 30 |
| `src/sdk/msl/__fp_trap.c` | __fp_trap | 256 | 84.98% | 30 |
| `src/game/main/menus/combat_map_draw_preview_block.c` | CombatMap_DrawPreviewBlock | 556 | 98.06% | 42 |
| `src/game/ov005/func_ov005_0235b5dc.c` | func_ov005_0235b5dc | 188 | 72.36% | 42 |
| `src/sdk/isd/isd_double_divide_by_ten.c` | func_0211fe0c | 1168 | 96.78% | 44 |
| `src/arm7/sdk/wm/wmsp_start_scan_ex.c` | func_arm7_027c342c | 960 | 98.44% | 65 |
| `src/sdk/msl/double2hex.c` | double2hex | 948 | 86.82% | 121 |
| `src/sdk/isd/isd_v_format.c` | Isd_VFormat | 4960 | 99.28% | 217 |
| `src/sdk/msl/__pformatter.c` | __pformatter | 2108 | 88.31% | 284 |
