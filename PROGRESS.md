# Decompilation progress

Written by `tools/progress.py --write PROGRESS.md` from `hooks/pre-commit`; do not edit.
A function counts as done when its file is marked `complete` in `delinks.txt`, so the
build links it from `src/`. Functions written as `asm` inside a .c file count separately.

## Totals

| Code | Functions in C | Bytes in C | % of bytes | Functions in asm | Bytes in asm |
|---|---:|---:|---:|---:|---:|
| game | 593 / 6603 | 98844 / 1531944 | 6.45% | 0 | 0 |
| SDK/library | 6 / 880 | 648 / 154050 | 0.42% | 1 | 30 |
| all | 599 / 7483 | 99492 / 1685994 | 5.90% | 1 | 30 |

## By module

Rows with nothing done are left out, but count in the totals.

| Module | Code | Functions in C | Bytes in C | % of bytes | Functions in asm |
|---|---|---:|---:|---:|---:|
| main | game | 570 / 5180 | 95412 / 1082632 | 8.81% | 0 |
| main | SDK/library | 1 / 767 | 40 / 130618 | 0.03% | 1 |
| itcm | SDK/library | 5 / 113 | 608 / 23432 | 2.59% | 0 |
| ov000 | game | 1 / 121 | 16 / 25300 | 0.06% | 0 |
| ov001 | game | 3 / 76 | 60 / 7472 | 0.80% | 0 |
| ov002 | game | 3 / 164 | 2736 / 64208 | 4.26% | 0 |
| ov003 | game | 2 / 235 | 64 / 82696 | 0.08% | 0 |
| ov004 | game | 12 / 236 | 344 / 67364 | 0.51% | 0 |
| ov005 | game | 1 / 270 | 108 / 96684 | 0.11% | 0 |
| ov006 | game | 1 / 321 | 104 / 105588 | 0.10% | 0 |

Modules with nothing done: none.

## Started, not linked

Source files in `src/` that the build does not use yet (not marked `complete`).

| File | Functions | Bytes |
|---|---|---:|
| `src/game/itcm/range_map/mark_adjacent_tiles.c` | RangeMap_MarkAdjacentTiles | 400 |
| `src/game/main/menus/campaign_map_init.c` | CampaignMap_Init | 3036 |
| `src/game/main/menus/menu_cursor_update.c` | MenuCursor_Update | 816 |
