# Decompilation progress

Written by `tools/progress.py --write PROGRESS.md` from `hooks/pre-commit`; do not edit.
A function counts as done when its file is marked `complete` in `delinks.txt`, so the
build links it from `src/`. Functions written as `asm` inside a .c file count separately.

## Totals

| Code | Functions in C | Bytes in C | % of bytes | Functions in asm | Bytes in asm |
|---|---:|---:|---:|---:|---:|
| game | 6609 / 6700 | 1482084 / 1553840 | 95.38% | 0 | 0 |
| SDK/library | 160 / 779 | 7144 / 132154 | 5.41% | 5 | 310 |
| all | 6769 / 7479 | 1489228 / 1685994 | 88.33% | 5 | 310 |

## By module

Rows with nothing done are left out, but count in the totals.

| Module | Code | Functions in C | Bytes in C | % of bytes | Functions in asm |
|---|---|---:|---:|---:|---:|
| main | game | 5131 / 5180 | 1036632 / 1082632 | 95.75% | 0 |
| main | SDK/library | 160 / 768 | 7144 / 130618 | 5.47% | 5 |
| itcm | game | 87 / 102 | 13496 / 21896 | 61.64% | 0 |
| ov000 | game | 119 / 120 | 25000 / 25300 | 98.81% | 0 |
| ov001 | game | 76 / 76 | 7472 / 7472 | 100.00% | 0 |
| ov002 | game | 152 / 164 | 53176 / 64208 | 82.82% | 0 |
| ov003 | game | 232 / 235 | 81820 / 82696 | 98.94% | 0 |
| ov004 | game | 232 / 236 | 64808 / 67364 | 96.21% | 0 |
| ov005 | game | 260 / 266 | 94692 / 96684 | 97.94% | 0 |
| ov006 | game | 320 / 321 | 104988 / 105588 | 99.43% | 0 |

Modules with nothing done: none.

## Started, not linked

Source files in `src/` that the build does not use yet (not marked `complete`).

| File | Functions | Bytes |
|---|---|---:|
| `src/game/itcm/range_map/load_move_costs.c` | MoveSearch_LoadMoveCosts | 224 |
| `src/game/itcm/range_map/mark_adjacent_tiles.c` | RangeMap_MarkAdjacentTiles | 400 |
| `src/game/main/ai/ai_build_rate_site_danger.c` | AiBuild_RateSiteDanger | 1736 |
| `src/game/main/ai/ai_oozium_move_to_target.c` | AiOozium_MoveToTarget | 1568 |
| `src/game/main/ai/ai_site_group_rate_surroundings.c` | AiSiteGroup_RateSurroundings | 1584 |
| `src/game/main/ai/ai_sites_collect.c` | AiSites_Collect | 376 |
| `src/game/main/ai/ai_unit_join_damaged_unit.c` | AiUnit_JoinDamagedUnit | 556 |
| `src/game/main/ai/ai_unit_upgrade_wait_action.c` | AiUnit_UpgradeWaitAction | 3924 |
| `src/game/main/ai/func_02093ce8.c` | func_02093ce8 | 524 |
| `src/game/main/ai/func_02099ba0.c` | func_02099ba0 | 524 |
| `src/game/main/ai/func_020a6ac4.c` | func_020a6ac4 | 620 |
| `src/game/main/copowers/func_02055f84.c` | func_02055f84 | 1412 |
| `src/game/main/copowers/tag_gauge_pop_in.c` | func_02057fb8 | 124 |
| `src/game/main/event_queue/event_queue_drain.c` | EventQueue_Drain | 192 |
| `src/game/main/func_02058b1c.c` | func_02058b1c | 1000 |
| `src/game/main/func_020593c8.c` | func_020593c8 | 1348 |
| `src/game/main/graphics/quad/quad_sprite_draw.c` | AnimSpriteTask_Draw | 484 |
| `src/game/main/link/func_02072a50.c` | func_02072a50 | 176 |
| `src/game/main/link/func_0207f550.c` | func_0207f550 | 668 |
| `src/game/main/link/link_screen_move_player_up.c` | LinkScreen_MovePlayerUp | 116 |
| `src/game/main/map/design_map_build_record.c` | DesignMap_BuildRecord | 796 |
| `src/game/main/map/func_020c0aa8.c` | func_020c0aa8 | 916 |
| `src/game/main/map/func_020c4448.c` | func_020c4448 | 904 |
| `src/game/main/map/func_020cb848.c` | func_020cb848 | 428 |
| `src/game/main/map/func_020f9450.c` | func_020f9450 | 240 |
| `src/game/main/map/func_020fed58.c` | func_020fed58 | 804 |
| `src/game/main/map/map_view_clear_unit_tile.c` | MapView_ClearUnitTile | 164 |
| `src/game/main/map/map_view_draw_unit_tile.c` | MapView_DrawUnitTile | 772 |
| `src/game/main/menus/campaign_map_init.c` | CampaignMap_Init | 3036 |
| `src/game/main/menus/co_select_screen_refresh_cell_skills.c` | CoSelectScreen_RefreshCellSkills | 132 |
| `src/game/main/menus/combat_map_draw_preview_block.c` | CombatMap_DrawPreviewBlock | 556 |
| `src/game/main/menus/combat_setup_add_shop_buttons.c` | func_020842e0 | 240 |
| `src/game/main/menus/func_0202f0d8.c` | func_0202f0d8 | 2164 |
| `src/game/main/menus/func_0204e808.c` | func_0204e808 | 248 |
| `src/game/main/menus/func_0204f0c0.c` | func_0204f0c0 | 160 |
| `src/game/main/menus/func_0204f160.c` | func_0204f160 | 160 |
| `src/game/main/menus/func_0204f200.c` | func_0204f200 | 160 |
| `src/game/main/menus/func_02064774.c` | func_02064774 | 132 |
| `src/game/main/menus/func_0206f3f8.c` | func_0206f3f8 | 616 |
| `src/game/main/menus/func_0208395c.c` | func_0208395c | 1444 |
| `src/game/main/menus/map_preview_draw_block.c` | MapPreview_DrawBlock | 788 |
| `src/game/main/screen/screen_apply_main.c` | Screen_ApplyMain | 3848 |
| `src/game/main/screen/screen_apply_sub.c` | Screen_ApplySub | 3624 |
| `src/game/main/screen/screen_prepare_h_blank_regs.c` | Screen_PrepareHBlankRegs | 1656 |
| `src/game/main/screen/window_wipe_start_hide_current.c` | WindowWipe_StartHideCurrent | 192 |
| `src/game/main/script/event_script_step_slot.c` | EventScript_StepSlot | 276 |
| `src/game/main/sound/sound_stream_fill_blocks.c` | SoundStream_FillBlocks | 2184 |
| `src/game/main/text/font_draw_glyph.c` | Font_DrawGlyph | 372 |
| `src/game/main/units/move_path_follow_cursor.c` | MovePath_FollowCursor | 916 |
| `src/game/main/units/unit_action_capture.c` | UnitAction_Capture | 1052 |
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
| `src/game/ov005/func_ov005_0235aec8.c` | func_ov005_0235aec8 | 640 |
| `src/game/ov005/func_ov005_0235b3d4.c` | func_ov005_0235b3d4 | 236 |
| `src/game/ov005/func_ov005_0235b4c0.c` | func_ov005_0235b4c0 | 180 |
| `src/game/ov005/func_ov005_0235b5dc.c` | func_ov005_0235b5dc | 188 |
| `src/game/ov005/func_ov005_02363014.c` | func_ov005_02363014 | 172 |
| `src/game/ov006/func_ov006_023557b8.c` | func_ov006_023557b8 | 600 |
| `src/sdk/gx/gx_set_bank_for_sub_bg.c` | func_0210ab74 | 172 |
| `src/sdk/gx/gx_set_bank_for_sub_bg_ext_pltt.c` | func_0210aa6c | 140 |
| `src/sdk/gx/gx_set_bank_for_sub_obj.c` | func_0210aaf8 | 124 |
| `src/sdk/gx/gx_set_bank_for_sub_obj_ext_pltt.c` | func_0210a9e0 | 140 |
