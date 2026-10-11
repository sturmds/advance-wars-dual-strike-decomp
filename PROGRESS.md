# Decompilation progress

Written by `tools/progress.py --write PROGRESS.md` from `hooks/pre-commit`; do not edit.
A function counts as done when its file is marked `complete` in `delinks.txt`, so the
build links it from `src/`. Functions written as `asm` inside a .c file count separately.

## Totals

| Code | Functions in C | Bytes in C | % of bytes | Functions in asm | Bytes in asm |
|---|---:|---:|---:|---:|---:|
| game | 5177 / 6598 | 1159996 / 1531944 | 75.72% | 0 | 0 |
| SDK/library | 139 / 881 | 3464 / 154050 | 2.25% | 1 | 30 |
| all | 5316 / 7479 | 1163460 / 1685994 | 69.01% | 1 | 30 |

## By module

Rows with nothing done are left out, but count in the totals.

| Module | Code | Functions in C | Bytes in C | % of bytes | Functions in asm |
|---|---|---:|---:|---:|---:|
| main | game | 3789 / 5180 | 728884 / 1082632 | 67.33% | 0 |
| main | SDK/library | 112 / 768 | 2392 / 130618 | 1.83% | 1 |
| itcm | SDK/library | 27 / 113 | 1072 / 23432 | 4.57% | 0 |
| ov000 | game | 119 / 120 | 25000 / 25300 | 98.81% | 0 |
| ov001 | game | 76 / 76 | 7472 / 7472 | 100.00% | 0 |
| ov002 | game | 151 / 164 | 52516 / 64208 | 81.79% | 0 |
| ov003 | game | 232 / 235 | 81820 / 82696 | 98.94% | 0 |
| ov004 | game | 232 / 236 | 64808 / 67364 | 96.21% | 0 |
| ov005 | game | 258 / 266 | 94508 / 96684 | 97.75% | 0 |
| ov006 | game | 320 / 321 | 104988 / 105588 | 99.43% | 0 |

Modules with nothing done: none.

## Started, not linked

Source files in `src/` that the build does not use yet (not marked `complete`).

| File | Functions | Bytes |
|---|---|---:|
| `src/game/itcm/range_map/mark_adjacent_tiles.c` | RangeMap_MarkAdjacentTiles | 400 |
| `src/game/main/ai/ai_oozium_move_to_target.c` | AiOozium_MoveToTarget | 1568 |
| `src/game/main/ai/ai_unit_join_damaged_unit.c` | AiUnit_JoinDamagedUnit | 556 |
| `src/game/main/ai/ai_unit_upgrade_wait_action.c` | AiUnit_UpgradeWaitAction | 3924 |
| `src/game/main/ai/func_02092350.c` | func_02092350 | 1584 |
| `src/game/main/ai/func_02093ce8.c` | func_02093ce8 | 524 |
| `src/game/main/ai/func_0209770c.c` | func_0209770c | 1736 |
| `src/game/main/ai/func_02099ba0.c` | func_02099ba0 | 524 |
| `src/game/main/copowers/func_02055f84.c` | func_02055f84 | 1412 |
| `src/game/main/func_0200c898.c` | func_0200c898 | 128 |
| `src/game/main/func_02058b1c.c` | func_02058b1c | 1000 |
| `src/game/main/func_020593c8.c` | func_020593c8 | 1348 |
| `src/game/main/func_020f20b4 5.c` | no delinks entry | 0 |
| `src/game/main/func_020f20fc 4.c` | no delinks entry | 0 |
| `src/game/main/func_020f2134 3.c` | no delinks entry | 0 |
| `src/game/main/func_020f6728.c` | no delinks entry | 0 |
| `src/game/main/func_020f678c.c` | no delinks entry | 0 |
| `src/game/main/func_020f67f0.c` | no delinks entry | 0 |
| `src/game/main/func_020f6854.c` | no delinks entry | 0 |
| `src/game/main/graphics/func_0200c500.c` | func_0200c500 | 216 |
| `src/game/main/graphics/sprite_anim_queue3d.c` | SpriteAnim_Queue3d | 196 |
| `src/game/main/link/func_020726ac.c` | func_020726ac | 116 |
| `src/game/main/map/func_020c0aa8.c` | func_020c0aa8 | 916 |
| `src/game/main/map/func_020c4448.c` | func_020c4448 | 904 |
| `src/game/main/map/func_020d73d4.c` | func_020d73d4 | 796 |
| `src/game/main/map/func_020fed58.c` | func_020fed58 | 804 |
| `src/game/main/menus/campaign_map_init.c` | CampaignMap_Init | 3036 |
| `src/game/main/menus/combat_setup_player_ready.c` | CombatSetup_SetPlayerReady | 148 |
| `src/game/main/menus/combat_setup_player_unready.c` | CombatSetup_SetPlayerUnready | 116 |
| `src/game/main/menus/func_0204e808.c` | func_0204e808 | 248 |
| `src/game/main/menus/func_0204e9cc.c` | func_0204e9cc | 164 |
| `src/game/main/menus/func_0204f0c0.c` | func_0204f0c0 | 160 |
| `src/game/main/menus/func_0204f160.c` | func_0204f160 | 160 |
| `src/game/main/menus/func_0204f200.c` | func_0204f200 | 160 |
| `src/game/main/menus/func_02082cf4.c` | func_02082cf4 | 556 |
| `src/game/main/menus/func_0208395c.c` | func_0208395c | 1444 |
| `src/game/main/menus/func_020d3dec.c` | func_020d3dec | 148 |
| `src/game/main/menus/map_preview_draw_block.c` | MapPreview_DrawBlock | 788 |
| `src/game/main/save_write_record.c` | Save_WriteRecord | 1428 |
| `src/game/main/screen/screen_apply_main.c` | Screen_ApplyMain | 3848 |
| `src/game/main/screen/screen_apply_sub.c` | Screen_ApplySub | 3624 |
| `src/game/main/screen/screen_prepare_h_blank_regs.c` | Screen_PrepareHBlankRegs | 1656 |
| `src/game/main/text/func_02020e0c.c` | func_02020e0c | 372 |
| `src/game/main/units/func_020d4748.c` | func_020d4748 | 916 |
| `src/game/main/units/unit_action_capture.c` | UnitAction_Capture | 1052 |
| `src/game/ov000/player_hud_draw_slot.c` | no delinks entry | 0 |
| `src/game/ov000/unit_info_panel/unit_info_panel_draw_page.c` | UnitInfoPanel_DrawPage | 300 |
| `src/game/ov002/battle_cell_sprite_arrived.c` | BattleCellSprite_Arrived | 312 |
| `src/game/ov002/battle_cell_sprite_start.c` | func_ov002_02357488 | 568 |
| `src/game/ov002/battle_fx_start_a.c` | func_ov002_02357e5c | 492 |
| `src/game/ov002/battle_make_fighters.c` | func_ov002_023571d0 | 356 |
| `src/game/ov002/func_ov002_02352754.c` | func_ov002_02352754 | 2564 |
| `src/game/ov002/func_ov002_02354814.c` | func_ov002_02354814 | 216 |
| `src/game/ov002/func_ov002_023549c8.c` | func_ov002_023549c8 | 2312 |
| `src/game/ov002/func_ov002_023552d0.c` | func_ov002_023552d0 | 1044 |
| `src/game/ov002/func_ov002_0235882c.c` | func_ov002_0235882c | 1044 |
| `src/game/ov002/func_ov002_02358c40.c` | func_ov002_02358c40 | 1372 |
| `src/game/ov002/func_ov002_0235d224.c` | func_ov002_0235d224 | 644 |
| `src/game/ov002/func_ov002_0235d4a8.c` | func_ov002_0235d4a8 | 108 |
| `src/game/ov003/func_ov003_0235bdd8.c` | func_ov003_0235bdd8 | 88 |
| `src/game/ov003/func_ov003_0235c194.c` | func_ov003_0235c194 | 88 |
| `src/game/ov003/func_ov003_02361c7c.c` | func_ov003_02361c7c | 700 |
| `src/game/ov004/func_ov004_022ad71c.c` | func_ov004_022ad71c | 352 |
| `src/game/ov004/func_ov004_022b1f98.c` | func_ov004_022b1f98 | 1736 |
| `src/game/ov004/func_ov004_022bacd8.c` | func_ov004_022bacd8 | 212 |
| `src/game/ov004/ov004_blit_block_with_flags.c` | Ov004_BlitBlockWithFlags | 256 |
| `src/game/ov005/func_ov005_0235a320.c` | func_ov005_0235a320 | 576 |
| `src/game/ov005/func_ov005_0235ac48.c` | func_ov005_0235ac48 | 132 |
| `src/game/ov005/func_ov005_0235b3d4.c` | func_ov005_0235b3d4 | 236 |
| `src/game/ov005/func_ov005_0235b4c0.c` | func_ov005_0235b4c0 | 180 |
| `src/game/ov005/func_ov005_0235b5dc.c` | func_ov005_0235b5dc | 188 |
| `src/game/ov005/func_ov005_02363014.c` | func_ov005_02363014 | 172 |
| `src/game/ov006/func_ov006_023557b8.c` | func_ov006_023557b8 | 600 |
| `src/game/ov006/func_ov006_0235e04c.c` | no delinks entry | 0 |
