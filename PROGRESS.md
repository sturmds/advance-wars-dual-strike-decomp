# Decompilation progress

Written by `tools/progress.py --write PROGRESS.md` from `hooks/pre-commit`; do not edit.
A function counts as done when its file is marked `complete` in `delinks.txt`, so the
build links it from `src/`. Functions written as `asm` inside a .c file count separately.

## Totals

| Code | Functions in C | Bytes in C | % of bytes | Functions in asm | Bytes in asm |
|---|---:|---:|---:|---:|---:|
| game | 1227 / 6603 | 193900 / 1531944 | 12.66% | 0 | 0 |
| SDK/library | 59 / 880 | 1728 / 154050 | 1.12% | 1 | 30 |
| all | 1286 / 7483 | 195628 / 1685994 | 11.60% | 1 | 30 |

## By module

Rows with nothing done are left out, but count in the totals.

| Module | Code | Functions in C | Bytes in C | % of bytes | Functions in asm |
|---|---|---:|---:|---:|---:|
| main | game | 1073 / 5180 | 184436 / 1082632 | 17.04% | 0 |
| main | SDK/library | 35 / 767 | 740 / 130618 | 0.57% | 1 |
| itcm | SDK/library | 24 / 113 | 988 / 23432 | 4.22% | 0 |
| ov000 | game | 4 / 121 | 652 / 25300 | 2.58% | 0 |
| ov001 | game | 3 / 76 | 60 / 7472 | 0.80% | 0 |
| ov002 | game | 12 / 164 | 4204 / 64208 | 6.55% | 0 |
| ov003 | game | 10 / 235 | 400 / 82696 | 0.48% | 0 |
| ov004 | game | 66 / 236 | 1736 / 67364 | 2.58% | 0 |
| ov005 | game | 49 / 270 | 1236 / 96684 | 1.28% | 0 |
| ov006 | game | 10 / 321 | 1176 / 105588 | 1.11% | 0 |

Modules with nothing done: none.

## Started, not linked

Source files in `src/` that the build does not use yet (not marked `complete`).

| File | Functions | Bytes |
|---|---|---:|
| `src/game/itcm/range_map/mark_adjacent_tiles.c` | RangeMap_MarkAdjacentTiles | 400 |
| `src/game/main/ai/ai_oozium_move_to_target.c` | AiOozium_MoveToTarget | 1568 |
| `src/game/main/ai/ai_unit_join_damaged_unit.c` | func_020aff0c | 556 |
| `src/game/main/ai/ai_unit_teammate_outdamages.c` | AiUnit_TeammateOutdamages | 772 |
| `src/game/main/ai/ai_unit_upgrade_wait_action.c` | AiUnit_UpgradeWaitAction | 3924 |
| `src/game/main/graphics/func_02001f50.c` | func_02001f50 | 88 |
| `src/game/main/map/func_020c0aa8.c` | func_020c0aa8 | 916 |
| `src/game/main/menus/campaign_map_init.c` | CampaignMap_Init | 3036 |
| `src/game/main/menus/map_preview_draw.c` | MapPreview_Draw | 880 |
| `src/game/main/screen/screen_apply_main.c` | Screen_ApplyMain | 3848 |
| `src/game/main/screen/screen_apply_sub.c` | Screen_ApplySub | 3624 |
| `src/game/main/screen/screen_prepare_h_blank_regs.c` | Screen_PrepareHBlankRegs | 1656 |
