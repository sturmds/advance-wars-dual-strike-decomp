# Decompilation progress

Written by `tools/progress.py --write PROGRESS.md` from `hooks/pre-commit`; do not edit.
A function counts as done when its file is marked `complete` in `delinks.txt`, so the
build links it from `src/`. Functions written as `asm` inside a .c file count separately.

## Totals

| Code | Functions in C | Bytes in C | % of bytes | Functions in asm | Bytes in asm |
|---|---:|---:|---:|---:|---:|
| game | 2032 / 6603 | 213364 / 1531944 | 13.93% | 0 | 0 |
| SDK/library | 136 / 880 | 3388 / 154050 | 2.20% | 1 | 30 |
| all | 2168 / 7483 | 216752 / 1685994 | 12.86% | 1 | 30 |

## By module

Rows with nothing done are left out, but count in the totals.

| Module | Code | Functions in C | Bytes in C | % of bytes | Functions in asm |
|---|---|---:|---:|---:|---:|
| main | game | 1702 / 5180 | 199784 / 1082632 | 18.45% | 0 |
| main | SDK/library | 109 / 767 | 2316 / 130618 | 1.77% | 1 |
| itcm | SDK/library | 27 / 113 | 1072 / 23432 | 4.57% | 0 |
| ov000 | game | 18 / 121 | 892 / 25300 | 3.53% | 0 |
| ov001 | game | 20 / 76 | 412 / 7472 | 5.51% | 0 |
| ov002 | game | 24 / 164 | 4528 / 64208 | 7.05% | 0 |
| ov003 | game | 27 / 235 | 800 / 82696 | 0.97% | 0 |
| ov004 | game | 101 / 236 | 2736 / 67364 | 4.06% | 0 |
| ov005 | game | 91 / 270 | 2140 / 96684 | 2.21% | 0 |
| ov006 | game | 49 / 321 | 2072 / 105588 | 1.96% | 0 |

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
