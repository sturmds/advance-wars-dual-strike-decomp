# Decompilation progress

Written by `tools/progress.py --write PROGRESS.md` from `hooks/pre-commit`; do not edit.
A function counts as done when its file is marked `complete` in `delinks.txt`, so the
build links it from `src/`. Functions written as `asm` inside a .c file count separately.

## Totals

| Code | Functions in C | Bytes in C | % of bytes | Functions in asm | Bytes in asm |
|---|---:|---:|---:|---:|---:|
| game | 759 / 6603 | 148240 / 1531944 | 9.68% | 0 | 0 |
| SDK/library | 6 / 880 | 648 / 154050 | 0.42% | 1 | 30 |
| all | 765 / 7483 | 148888 / 1685994 | 8.83% | 1 | 30 |

## By module

Rows with nothing done are left out, but count in the totals.

| Module | Code | Functions in C | Bytes in C | % of bytes | Functions in asm |
|---|---|---:|---:|---:|---:|
| main | game | 718 / 5180 | 144448 / 1082632 | 13.34% | 0 |
| main | SDK/library | 1 / 767 | 40 / 130618 | 0.03% | 1 |
| itcm | SDK/library | 5 / 113 | 608 / 23432 | 2.59% | 0 |
| ov000 | game | 1 / 121 | 16 / 25300 | 0.06% | 0 |
| ov001 | game | 3 / 76 | 60 / 7472 | 0.80% | 0 |
| ov002 | game | 4 / 164 | 2764 / 64208 | 4.30% | 0 |
| ov003 | game | 4 / 235 | 104 / 82696 | 0.13% | 0 |
| ov004 | game | 12 / 236 | 344 / 67364 | 0.51% | 0 |
| ov005 | game | 12 / 270 | 332 / 96684 | 0.34% | 0 |
| ov006 | game | 5 / 321 | 172 / 105588 | 0.16% | 0 |

Modules with nothing done: none.

## Started, not linked

Source files in `src/` that the build does not use yet (not marked `complete`).

| File | Functions | Bytes |
|---|---|---:|
| `src/game/itcm/range_map/mark_adjacent_tiles.c` | RangeMap_MarkAdjacentTiles | 400 |
| `src/game/main/ai/ai_unit_join_damaged_unit.c` | func_020aff0c | 556 |
| `src/game/main/ai/ai_unit_teammate_outdamages.c` | AiUnit_TeammateOutdamages | 772 |
| `src/game/main/ai/ai_unit_upgrade_wait_action.c` | AiUnit_UpgradeWaitAction | 3924 |
| `src/game/main/menus/campaign_map_init.c` | CampaignMap_Init | 3036 |
| `src/game/main/menus/map_preview_draw.c` | MapPreview_Draw | 880 |
| `src/game/main/screen/screen_apply_main.c` | Screen_ApplyMain | 3848 |
