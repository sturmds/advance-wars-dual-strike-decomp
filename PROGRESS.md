# Decompilation progress

Written by `tools/progress.py --write PROGRESS.md` from `hooks/pre-commit`; do not edit.
A function counts as done when its file is marked `complete` in `delinks.txt`, so the
build links it from `src/`. Functions written as `asm` inside a .c file count separately.

## Totals

ARM9 is main, ITCM and the overlays; ARM7 is `arm7.bin`. `game`, `SDK/library` and `all`
add both CPUs.

| Code | Functions in C | Bytes in C | % of bytes | Functions in asm | Bytes in asm |
|---|---:|---:|---:|---:|---:|
| ARM9 game | 6663 / 6700 | 1512952 / 1553840 | 97.37% | 0 | 0 |
| ARM9 SDK/library | 640 / 833 | 101304 / 135226 | 74.91% | 176 | 20718 |
| ARM9 all | 7303 / 7533 | 1614256 / 1689066 | 95.57% | 176 | 20718 |
| ARM7 SDK/library | 908 / 1020 | 123104 / 154300 | 79.78% | 69 | 4960 |
| ARM7 all | 908 / 1020 | 123104 / 154300 | 79.78% | 69 | 4960 |
| game | 6663 / 6700 | 1512952 / 1553840 | 97.37% | 0 | 0 |
| SDK/library | 1548 / 1853 | 224408 / 289526 | 77.51% | 245 | 25678 |
| all | 8211 / 8553 | 1737360 / 1843366 | 94.25% | 245 | 25678 |

## By module

Rows with nothing done are left out, but count in the totals; ARM7's row is always shown.

| Module | Code | Functions in C | Bytes in C | % of bytes | Functions in asm |
|---|---|---:|---:|---:|---:|
| main | game | 5157 / 5180 | 1054052 / 1082632 | 97.36% | 0 |
| main | SDK/library | 633 / 822 | 100392 / 133690 | 75.09% | 172 |
| itcm | game | 102 / 102 | 21896 / 21896 | 100.00% | 0 |
| itcm | SDK/library | 7 / 11 | 912 / 1536 | 59.38% | 4 |
| ov000 | game | 119 / 120 | 25000 / 25300 | 98.81% | 0 |
| ov001 | game | 76 / 76 | 7472 / 7472 | 100.00% | 0 |
| ov002 | game | 159 / 164 | 56424 / 64208 | 87.88% | 0 |
| ov003 | game | 234 / 235 | 81996 / 82696 | 99.15% | 0 |
| ov004 | game | 232 / 236 | 64808 / 67364 | 96.21% | 0 |
| ov005 | game | 264 / 266 | 96316 / 96684 | 99.62% | 0 |
| ov006 | game | 320 / 321 | 104988 / 105588 | 99.43% | 0 |
| arm7 | SDK/library | 908 / 1020 | 123104 / 154300 | 79.78% | 69 |

Modules with nothing done: none.

## Started, not linked

Source files in `src/` that the build does not use yet (not marked `complete`).

| File | Functions | Bytes |
|---|---|---:|
| `src/arm7/sdk/wm/func_arm7_027c6020.c` | func_arm7_027c6020 | 152 |
| `src/arm7/sdk/wm/func_arm7_027c632c.c` | func_arm7_027c632c | 484 |
| `src/game/main/ai/ai_build_rate_site_danger.c` | AiBuild_RateSiteDanger | 1736 |
| `src/game/main/ai/ai_oozium_move_to_target.c` | AiOozium_MoveToTarget | 1568 |
| `src/game/main/ai/ai_site_group_rate_surroundings.c` | AiSiteGroup_RateSurroundings | 1584 |
| `src/game/main/ai/ai_unit_upgrade_wait_action.c` | AiUnit_UpgradeWaitAction | 3924 |
| `src/game/main/ai/func_02093ce8.c` | func_02093ce8 | 524 |
| `src/game/main/ai/func_02099ba0.c` | func_02099ba0 | 524 |
| `src/game/main/ai/func_020a6ac4.c` | func_020a6ac4 | 620 |
| `src/game/main/func_02058b1c.c` | func_02058b1c | 1000 |
| `src/game/main/graphics/quad/quad_sprite_draw.c` | AnimSpriteTask_Draw | 484 |
| `src/game/main/link/func_0207f550.c` | func_0207f550 | 668 |
| `src/game/main/map/func_020c0aa8.c` | func_020c0aa8 | 916 |
| `src/game/main/map/func_020cb848.c` | func_020cb848 | 428 |
| `src/game/main/map/func_020f9450.c` | func_020f9450 | 240 |
| `src/game/main/menus/campaign_map_init.c` | CampaignMap_Init | 3036 |
| `src/game/main/menus/combat_map_draw_preview_block.c` | CombatMap_DrawPreviewBlock | 556 |
| `src/game/main/menus/func_0204e808.c` | func_0204e808 | 248 |
| `src/game/main/menus/func_0204f0c0.c` | func_0204f0c0 | 160 |
| `src/game/main/menus/func_0204f160.c` | func_0204f160 | 160 |
| `src/game/main/menus/func_0204f200.c` | func_0204f200 | 160 |
| `src/game/main/screen/screen_apply_main.c` | Screen_ApplyMain | 3848 |
| `src/game/main/screen/screen_apply_sub.c` | Screen_ApplySub | 3624 |
| `src/game/main/screen/screen_prepare_h_blank_regs.c` | Screen_PrepareHBlankRegs | 1656 |
| `src/game/main/units/move_path_follow_cursor.c` | MovePath_FollowCursor | 916 |
| `src/game/ov000/unit_info_panel/unit_info_panel_draw_page.c` | UnitInfoPanel_DrawPage | 300 |
| `src/game/ov002/battle_fx_start_a.c` | func_ov002_02357e5c | 492 |
| `src/game/ov002/func_ov002_02352754.c` | func_ov002_02352754 | 2564 |
| `src/game/ov002/func_ov002_023549c8.c` | func_ov002_023549c8 | 2312 |
| `src/game/ov002/func_ov002_023552d0.c` | func_ov002_023552d0 | 1044 |
| `src/game/ov002/func_ov002_02358c40.c` | func_ov002_02358c40 | 1372 |
| `src/game/ov003/func_ov003_02361c7c.c` | func_ov003_02361c7c | 700 |
| `src/game/ov004/func_ov004_022ad71c.c` | func_ov004_022ad71c | 352 |
| `src/game/ov004/func_ov004_022b1f98.c` | func_ov004_022b1f98 | 1736 |
| `src/game/ov004/func_ov004_022bacd8.c` | func_ov004_022bacd8 | 212 |
| `src/game/ov004/ov004_blit_block_with_flags.c` | Ov004_BlitBlockWithFlags | 256 |
| `src/game/ov005/func_ov005_0235b4c0.c` | func_ov005_0235b4c0 | 180 |
| `src/game/ov005/func_ov005_0235b5dc.c` | func_ov005_0235b5dc | 188 |
| `src/game/ov006/func_ov006_023557b8.c` | func_ov006_023557b8 | 600 |
| `src/sdk/card/cardi_set_backup_spec.c` | func_02115858 | 472 |
| `src/sdk/card/cardi_start_rom_dma.c` | func_021162d8 | 340 |
| `src/sdk/card/func_021154e8.c` | func_021154e8 | 252 |
| `src/sdk/isd/isd_double_add.c` | func_02120574 | 920 |
| `src/sdk/isd/isd_double_divide_by_ten.c` | func_0211fe0c | 1168 |
| `src/sdk/isd/isd_double_times_ten.c` | func_0212029c | 728 |
| `src/sdk/isd/isd_v_format.c` | Isd_VFormat | 4960 |
| `src/sdk/msl/__fp_trap.c` | __fp_trap | 256 |
| `src/sdk/msl/__fp_trap_d.c` | __fp_trap_d | 80 |
| `src/sdk/msl/__fp_trap_dd.c` | __fp_trap_dd | 92 |
| `src/sdk/msl/__fp_trap_f.c` | __fp_trap_f | 72 |
| `src/sdk/msl/__fp_trap_ff.c` | __fp_trap_ff | 76 |
| `src/sdk/msl/__pformatter.c` | __pformatter | 2108 |
| `src/sdk/msl/double2hex.c` | double2hex | 948 |
| `src/sdk/msl/func_02121d58.c` | func_02121d58 | 88 |
| `src/sdk/msl/func_02121ea8.c` | func_02121ea8 | 76 |
| `src/sdk/pm/pm_go_sleep_mode.c` | func_02114ae8 | 568 |
