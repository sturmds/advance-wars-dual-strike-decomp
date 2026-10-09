# Decompilation progress

Written by `tools/progress.py --write PROGRESS.md` from `hooks/pre-commit`; do not edit.
A function counts as done when its file is marked `complete` in `delinks.txt`, so the
build links it from `src/`. Functions written as `asm` inside a .c file count separately.

## Totals

| Code | Functions in C | Bytes in C | % of bytes | Functions in asm | Bytes in asm |
|---|---:|---:|---:|---:|---:|
| game | 930 / 6603 | 186952 / 1531944 | 12.20% | 0 | 0 |
| SDK/library | 6 / 880 | 648 / 154050 | 0.42% | 1 | 30 |
| all | 936 / 7483 | 187600 / 1685994 | 11.13% | 1 | 30 |

## By module

Rows with nothing done are left out, but count in the totals.

| Module | Code | Functions in C | Bytes in C | % of bytes | Functions in asm |
|---|---|---:|---:|---:|---:|
| main | game | 876 / 5180 | 180028 / 1082632 | 16.63% | 0 |
| main | SDK/library | 1 / 767 | 40 / 130618 | 0.03% | 1 |
| itcm | SDK/library | 5 / 113 | 608 / 23432 | 2.59% | 0 |
| ov000 | game | 3 / 121 | 628 / 25300 | 2.48% | 0 |
| ov001 | game | 3 / 76 | 60 / 7472 | 0.80% | 0 |
| ov002 | game | 10 / 164 | 4136 / 64208 | 6.44% | 0 |
| ov003 | game | 5 / 235 | 260 / 82696 | 0.31% | 0 |
| ov004 | game | 12 / 236 | 344 / 67364 | 0.51% | 0 |
| ov005 | game | 12 / 270 | 332 / 96684 | 0.34% | 0 |
| ov006 | game | 9 / 321 | 1164 / 105588 | 1.10% | 0 |

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
