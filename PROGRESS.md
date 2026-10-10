# Decompilation progress

Written by `tools/progress.py --write PROGRESS.md` from `hooks/pre-commit`; do not edit.
A function counts as done when its file is marked `complete` in `delinks.txt`, so the
build links it from `src/`. Functions written as `asm` inside a .c file count separately.

## Totals

| Code | Functions in C | Bytes in C | % of bytes | Functions in asm | Bytes in asm |
|---|---:|---:|---:|---:|---:|
| game | 2874 / 6603 | 405052 / 1531944 | 26.44% | 0 | 0 |
| SDK/library | 138 / 880 | 3420 / 154050 | 2.22% | 1 | 30 |
| all | 3012 / 7483 | 408472 / 1685994 | 24.23% | 1 | 30 |

## By module

Rows with nothing done are left out, but count in the totals.

| Module | Code | Functions in C | Bytes in C | % of bytes | Functions in asm |
|---|---|---:|---:|---:|---:|
| main | game | 2359 / 5180 | 339992 / 1082632 | 31.40% | 0 |
| main | SDK/library | 111 / 767 | 2348 / 130618 | 1.80% | 1 |
| itcm | SDK/library | 27 / 113 | 1072 / 23432 | 4.57% | 0 |
| ov000 | game | 64 / 121 | 15824 / 25300 | 62.55% | 0 |
| ov001 | game | 35 / 76 | 2208 / 7472 | 29.55% | 0 |
| ov002 | game | 81 / 164 | 23088 / 64208 | 35.96% | 0 |
| ov003 | game | 45 / 235 | 6932 / 82696 | 8.38% | 0 |
| ov004 | game | 105 / 236 | 4104 / 67364 | 6.09% | 0 |
| ov005 | game | 103 / 270 | 5700 / 96684 | 5.90% | 0 |
| ov006 | game | 82 / 321 | 7204 / 105588 | 6.82% | 0 |

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
| `src/game/ov000/player_hud_draw_slot.c` | no delinks entry | 0 |
| `src/game/ov000/unit_info_panel/unit_info_panel_draw_page.c` | func_ov000_022b2114 | 300 |
| `src/game/ov001/func_ov001_02351538.c` | func_ov001_02351538 | 264 |
| `src/game/ov002/battle_make_fighters.c` | func_ov002_023571d0 | 356 |
| `src/game/ov002/func_ov002_02353254.c` | func_ov002_02353254 | 308 |
| `src/game/ov002/func_ov002_02353c08.c` | func_ov002_02353c08 | 392 |
| `src/game/ov002/func_ov002_02354814.c` | func_ov002_02354814 | 216 |
