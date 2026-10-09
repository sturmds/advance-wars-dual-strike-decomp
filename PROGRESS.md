# Decompilation progress

Written by `tools/progress.py --write PROGRESS.md` from `hooks/pre-commit`; do not edit.
A function counts as done when its file is marked `complete` in `delinks.txt`, so the
build links it from `src/`. Functions written as `asm` inside a .c file count separately.

## Totals

| Code | Functions in C | Bytes in C | % of bytes | Functions in asm | Bytes in asm |
|---|---:|---:|---:|---:|---:|
| game | 1713 / 6603 | 204652 / 1531944 | 13.36% | 0 | 0 |
| SDK/library | 107 / 880 | 2864 / 154050 | 1.86% | 1 | 30 |
| all | 1820 / 7483 | 207516 / 1685994 | 12.31% | 1 | 30 |

## By module

Rows with nothing done are left out, but count in the totals.

| Module | Code | Functions in C | Bytes in C | % of bytes | Functions in asm |
|---|---|---:|---:|---:|---:|
| main | game | 1475 / 5180 | 193360 / 1082632 | 17.86% | 0 |
| main | SDK/library | 81 / 767 | 1808 / 130618 | 1.38% | 1 |
| itcm | SDK/library | 26 / 113 | 1056 / 23432 | 4.51% | 0 |
| ov000 | game | 16 / 121 | 836 / 25300 | 3.30% | 0 |
| ov001 | game | 17 / 76 | 364 / 7472 | 4.87% | 0 |
| ov002 | game | 19 / 164 | 4384 / 64208 | 6.83% | 0 |
| ov003 | game | 17 / 235 | 552 / 82696 | 0.67% | 0 |
| ov004 | game | 72 / 236 | 1816 / 67364 | 2.70% | 0 |
| ov005 | game | 67 / 270 | 1612 / 96684 | 1.67% | 0 |
| ov006 | game | 30 / 321 | 1728 / 105588 | 1.64% | 0 |

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
