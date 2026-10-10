# Decompilation progress

Written by `tools/progress.py --write PROGRESS.md` from `hooks/pre-commit`; do not edit.
A function counts as done when its file is marked `complete` in `delinks.txt`, so the
build links it from `src/`. Functions written as `asm` inside a .c file count separately.

## Totals

| Code | Functions in C | Bytes in C | % of bytes | Functions in asm | Bytes in asm |
|---|---:|---:|---:|---:|---:|
| game | 2758 / 6603 | 370864 / 1531944 | 24.21% | 0 | 0 |
| SDK/library | 138 / 880 | 3420 / 154050 | 2.22% | 1 | 30 |
| all | 2896 / 7483 | 374284 / 1685994 | 22.20% | 1 | 30 |

## By module

Rows with nothing done are left out, but count in the totals.

| Module | Code | Functions in C | Bytes in C | % of bytes | Functions in asm |
|---|---|---:|---:|---:|---:|
| main | game | 2342 / 5180 | 337608 / 1082632 | 31.18% | 0 |
| main | SDK/library | 111 / 767 | 2348 / 130618 | 1.80% | 1 |
| itcm | SDK/library | 27 / 113 | 1072 / 23432 | 4.57% | 0 |
| ov000 | game | 31 / 121 | 2740 / 25300 | 10.83% | 0 |
| ov001 | game | 22 / 76 | 952 / 7472 | 12.74% | 0 |
| ov002 | game | 44 / 164 | 9948 / 64208 | 15.49% | 0 |
| ov003 | game | 37 / 235 | 5304 / 82696 | 6.41% | 0 |
| ov004 | game | 102 / 236 | 2844 / 67364 | 4.22% | 0 |
| ov005 | game | 100 / 270 | 4536 / 96684 | 4.69% | 0 |
| ov006 | game | 80 / 321 | 6932 / 105588 | 6.57% | 0 |

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
| `src/game/ov001/func_ov001_02351538.c` | func_ov001_02351538 | 264 |
