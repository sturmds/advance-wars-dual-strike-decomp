# Decompilation progress

Written by `tools/progress.py --write PROGRESS.md` from `hooks/pre-commit`; do not edit.
A function counts as done when its file is marked `complete` in `delinks.txt`, so the
build links it from `src/`. Functions written as `asm` inside a .c file count separately.

## Totals

| Code | Functions in C | Bytes in C | % of bytes | Functions in asm | Bytes in asm |
|---|---:|---:|---:|---:|---:|
| game | 2715 / 6603 | 347048 / 1531944 | 22.65% | 0 | 0 |
| SDK/library | 138 / 880 | 3420 / 154050 | 2.22% | 1 | 30 |
| all | 2853 / 7483 | 350468 / 1685994 | 20.79% | 1 | 30 |

## By module

Rows with nothing done are left out, but count in the totals.

| Module | Code | Functions in C | Bytes in C | % of bytes | Functions in asm |
|---|---|---:|---:|---:|---:|
| main | game | 2318 / 5180 | 321024 / 1082632 | 29.65% | 0 |
| main | SDK/library | 111 / 767 | 2348 / 130618 | 1.80% | 1 |
| itcm | SDK/library | 27 / 113 | 1072 / 23432 | 4.57% | 0 |
| ov000 | game | 31 / 121 | 2740 / 25300 | 10.83% | 0 |
| ov001 | game | 20 / 76 | 412 / 7472 | 5.51% | 0 |
| ov002 | game | 36 / 164 | 6224 / 64208 | 9.69% | 0 |
| ov003 | game | 36 / 235 | 5224 / 82696 | 6.32% | 0 |
| ov004 | game | 102 / 236 | 2844 / 67364 | 4.22% | 0 |
| ov005 | game | 96 / 270 | 2360 / 96684 | 2.44% | 0 |
| ov006 | game | 76 / 321 | 6220 / 105588 | 5.89% | 0 |

Modules with nothing done: none.

## Started, not linked

Source files in `src/` that the build does not use yet (not marked `complete`).

| File | Functions | Bytes |
|---|---|---:|
| `src/game/itcm/range_map/mark_adjacent_tiles.c` | RangeMap_MarkAdjacentTiles | 400 |
| `src/game/main/ai/ai_add_oozium_threat.c` | Ai_AddOoziumThreat | 492 |
| `src/game/main/ai/ai_foot_board_transport_for_site.c` | AiFoot_BoardTransportForSite | 1040 |
| `src/game/main/ai/ai_oozium_move_to_target.c` | AiOozium_MoveToTarget | 1568 |
| `src/game/main/ai/ai_unit_has_fuel_for_tile.c` | func_020a6554 | 528 |
| `src/game/main/ai/ai_unit_join_damaged_unit.c` | func_020aff0c | 556 |
| `src/game/main/ai/ai_unit_teammate_outdamages.c` | AiUnit_TeammateOutdamages | 772 |
| `src/game/main/ai/ai_unit_upgrade_wait_action.c` | AiUnit_UpgradeWaitAction | 3924 |
| `src/game/main/ai/func_02091e40.c` | func_02091e40 | 112 |
| `src/game/main/ai/func_0209462c.c` | func_0209462c | 976 |
| `src/game/main/ai/func_0209b458.c` | func_0209b458 | 1216 |
| `src/game/main/ai/func_020a7034.c` | func_020a7034 | 944 |
| `src/game/main/ai/func_020aa4dc.c` | func_020aa4dc | 1256 |
| `src/game/main/ai/func_020ab70c.c` | func_020ab70c | 796 |
| `src/game/main/ai/func_020b46c4.c` | func_020b46c4 | 644 |
| `src/game/main/ai/func_020b7e90.c` | func_020b7e90 | 436 |
| `src/game/main/copowers/player_add_power_charge.c` | Player_AddPowerCharge | 432 |
| `src/game/main/func_0200c898.c` | func_0200c898 | 128 |
| `src/game/main/link_send_packet.c` | no delinks entry | 0 |
| `src/game/main/map/func_020c0aa8.c` | func_020c0aa8 | 916 |
| `src/game/main/map/func_020c4448.c` | func_020c4448 | 904 |
| `src/game/main/map/func_020fed58.c` | func_020fed58 | 804 |
| `src/game/main/map/map_view_scroll_step.c` | MapView_ScrollStep | 788 |
| `src/game/main/map/tile_map_fill.c` | TileMap_Fill | 244 |
| `src/game/main/menus/campaign_map_init.c` | CampaignMap_Init | 3036 |
| `src/game/main/menus/map_preview_draw_block.c` | MapPreview_DrawBlock | 788 |
| `src/game/main/screen/screen_apply_main.c` | Screen_ApplyMain | 3848 |
| `src/game/main/screen/screen_apply_sub.c` | Screen_ApplySub | 3624 |
| `src/game/main/screen/screen_prepare_h_blank_regs.c` | Screen_PrepareHBlankRegs | 1656 |
| `src/game/main/units/unit_action_capture.c` | UnitAction_Capture | 1052 |
