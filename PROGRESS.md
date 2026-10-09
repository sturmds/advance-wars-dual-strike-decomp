# Decompilation progress

Written by `tools/progress.py --write PROGRESS.md` from `hooks/pre-commit`; do not edit.
A function counts as done when its file is marked `complete` in `delinks.txt`, so the
build links it from `src/`. Functions written as `asm` inside a .c file count separately.

## Totals

| Code | Functions in C | Bytes in C | % of bytes | Functions in asm | Bytes in asm |
|---|---:|---:|---:|---:|---:|
| game | 1397 / 6603 | 195476 / 1531944 | 12.76% | 0 | 0 |
| SDK/library | 68 / 880 | 1796 / 154050 | 1.17% | 1 | 30 |
| all | 1465 / 7483 | 197272 / 1685994 | 11.70% | 1 | 30 |

## By module

Rows with nothing done are left out, but count in the totals.

| Module | Code | Functions in C | Bytes in C | % of bytes | Functions in asm |
|---|---|---:|---:|---:|---:|
| main | game | 1221 / 5180 | 185852 / 1082632 | 17.17% | 0 |
| main | SDK/library | 44 / 767 | 808 / 130618 | 0.62% | 1 |
| itcm | SDK/library | 24 / 113 | 988 / 23432 | 4.22% | 0 |
| ov000 | game | 11 / 121 | 704 / 25300 | 2.78% | 0 |
| ov001 | game | 5 / 76 | 84 / 7472 | 1.12% | 0 |
| ov002 | game | 13 / 164 | 4216 / 64208 | 6.57% | 0 |
| ov003 | game | 13 / 235 | 428 / 82696 | 0.52% | 0 |
| ov004 | game | 69 / 236 | 1756 / 67364 | 2.61% | 0 |
| ov005 | game | 53 / 270 | 1252 / 96684 | 1.29% | 0 |
| ov006 | game | 12 / 321 | 1184 / 105588 | 1.12% | 0 |

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
