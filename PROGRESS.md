# Decompilation progress

Written by `tools/progress.py --write PROGRESS.md` from `hooks/pre-commit`; do not edit.
A function counts as done when its file is marked `complete` in `delinks.txt`, so the
build links it from `src/`. Functions written as `asm` inside a .c file count separately.

## Totals

| Code | Functions in C | Bytes in C | % of bytes | Functions in asm | Bytes in asm |
|---|---:|---:|---:|---:|---:|
| game | 3397 / 6603 | 560940 / 1531944 | 36.62% | 0 | 0 |
| SDK/library | 138 / 881 | 3420 / 154050 | 2.22% | 1 | 30 |
| all | 3535 / 7484 | 564360 / 1685994 | 33.47% | 1 | 30 |

## By module

Rows with nothing done are left out, but count in the totals.

| Module | Code | Functions in C | Bytes in C | % of bytes | Functions in asm |
|---|---|---:|---:|---:|---:|
| main | game | 2418 / 5180 | 351184 / 1082632 | 32.44% | 0 |
| main | SDK/library | 111 / 768 | 2348 / 130618 | 1.80% | 1 |
| itcm | SDK/library | 27 / 113 | 1072 / 23432 | 4.57% | 0 |
| ov000 | game | 117 / 121 | 23924 / 25300 | 94.56% | 0 |
| ov001 | game | 73 / 76 | 6832 / 7472 | 91.43% | 0 |
| ov002 | game | 146 / 164 | 50440 / 64208 | 78.56% | 0 |
| ov003 | game | 228 / 235 | 79720 / 82696 | 96.40% | 0 |
| ov004 | game | 191 / 236 | 24072 / 67364 | 35.73% | 0 |
| ov005 | game | 139 / 270 | 17364 / 96684 | 17.96% | 0 |
| ov006 | game | 85 / 321 | 7404 / 105588 | 7.01% | 0 |

Modules with nothing done: none.

## Started, not linked

Source files in `src/` that the build does not use yet (not marked `complete`).

| File | Functions | Bytes |
|---|---|---:|
| `src/game/itcm/range_map/mark_adjacent_tiles.c` | RangeMap_MarkAdjacentTiles | 400 |
| `src/game/main/ai/ai_oozium_move_to_target.c` | AiOozium_MoveToTarget | 1568 |
| `src/game/main/ai/ai_unit_join_damaged_unit.c` | AiUnit_JoinDamagedUnit | 556 |
| `src/game/main/ai/ai_unit_upgrade_wait_action.c` | AiUnit_UpgradeWaitAction | 3924 |
| `src/game/main/ai/func_02099ba0.c` | func_02099ba0 | 524 |
| `src/game/main/func_0200c898.c` | func_0200c898 | 128 |
| `src/game/main/map/func_020c0aa8.c` | func_020c0aa8 | 916 |
| `src/game/main/map/func_020c4448.c` | func_020c4448 | 904 |
| `src/game/main/map/func_020fed58.c` | func_020fed58 | 804 |
| `src/game/main/menus/campaign_map_init.c` | CampaignMap_Init | 3036 |
| `src/game/main/menus/map_preview_draw_block.c` | MapPreview_DrawBlock | 788 |
| `src/game/main/screen/screen_apply_main.c` | Screen_ApplyMain | 3848 |
| `src/game/main/screen/screen_apply_sub.c` | Screen_ApplySub | 3624 |
| `src/game/main/screen/screen_prepare_h_blank_regs.c` | Screen_PrepareHBlankRegs | 1656 |
| `src/game/main/units/unit_action_capture.c` | UnitAction_Capture | 1052 |
| `src/game/ov000/func_ov000_022afbf0.c` | func_ov000_022afbf0 | 760 |
| `src/game/ov000/func_ov000_022afee8.c` | func_ov000_022afee8 | 212 |
| `src/game/ov000/player_hud_draw_slot.c` | no delinks entry | 0 |
| `src/game/ov000/task/unit_path_count_steps.c` | func_ov000_022ad560 | 104 |
| `src/game/ov000/unit_info_panel/unit_info_panel_draw_page.c` | UnitInfoPanel_DrawPage | 300 |
| `src/game/ov001/func_ov001_02351174.c` | func_ov001_02351174 | 244 |
| `src/game/ov001/func_ov001_02351268.c` | func_ov001_02351268 | 132 |
| `src/game/ov001/func_ov001_02351538.c` | func_ov001_02351538 | 264 |
| `src/game/ov002/battle_cell_sprite_arrived.c` | BattleCellSprite_Arrived | 312 |
| `src/game/ov002/battle_cell_sprite_start.c` | func_ov002_02357488 | 568 |
| `src/game/ov002/battle_draw_textured_quad.c` | Battle_DrawTexturedQuad | 392 |
| `src/game/ov002/battle_fighter_task_setup.c` | BattleFighterTask_Setup | 544 |
| `src/game/ov002/battle_footstep_task_start.c` | BattleFootstepTask_Start | 712 |
| `src/game/ov002/battle_fx_start_a.c` | func_ov002_02357e5c | 492 |
| `src/game/ov002/battle_make_fighters.c` | func_ov002_023571d0 | 356 |
| `src/game/ov002/func_ov002_02352754.c` | func_ov002_02352754 | 2564 |
| `src/game/ov002/func_ov002_02353254.c` | func_ov002_02353254 | 308 |
| `src/game/ov002/func_ov002_02354814.c` | func_ov002_02354814 | 216 |
| `src/game/ov002/func_ov002_023549c8.c` | func_ov002_023549c8 | 2312 |
| `src/game/ov002/func_ov002_023552d0.c` | func_ov002_023552d0 | 1044 |
| `src/game/ov002/func_ov002_0235882c.c` | func_ov002_0235882c | 1044 |
| `src/game/ov002/func_ov002_02358c40.c` | func_ov002_02358c40 | 1372 |
| `src/game/ov002/func_ov002_0235d224.c` | func_ov002_0235d224 | 644 |
| `src/game/ov002/func_ov002_0235d4a8.c` | func_ov002_0235d4a8 | 108 |
| `src/game/ov002/func_ov002_0235fc68.c` | func_ov002_0235fc68 | 120 |
| `src/game/ov003/func_ov003_0235bdd8.c` | func_ov003_0235bdd8 | 88 |
| `src/game/ov003/func_ov003_0235c194.c` | func_ov003_0235c194 | 88 |
| `src/game/ov003/func_ov003_0235fdd8.c` | no delinks entry | 0 |
| `src/game/ov003/func_ov003_02364514.c` | no delinks entry | 0 |
| `src/game/ov003/ov003_find_free_tile_near.c` | no delinks entry | 0 |
| `src/game/ov004/func_ov004_022ad71c.c` | func_ov004_022ad71c | 352 |
