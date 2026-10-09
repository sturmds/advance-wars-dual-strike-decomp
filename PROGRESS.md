# Decompilation progress

Written by `tools/progress.py --write PROGRESS.md` from `hooks/pre-commit`; do not edit.
A function counts as done when its file is marked `complete` in `delinks.txt`, so the
build links it from `src/`. Functions written as `asm` inside a .c file count separately.

## Totals

| Code | Functions in C | Bytes in C | % of bytes | Functions in asm | Bytes in asm |
|---|---:|---:|---:|---:|---:|
| game | 205 / 6603 | 37972 / 1531944 | 2.48% | 0 | 0 |
| SDK/library | 1 / 880 | 40 / 154050 | 0.03% | 1 | 30 |
| all | 206 / 7483 | 38012 / 1685994 | 2.25% | 1 | 30 |

## By module

Rows with nothing done are left out, but count in the totals.

| Module | Code | Functions in C | Bytes in C | % of bytes | Functions in asm |
|---|---|---:|---:|---:|---:|
| main | game | 199 / 5180 | 36536 / 1082632 | 3.37% | 0 |
| main | SDK/library | 1 / 767 | 40 / 130618 | 0.03% | 1 |
| ov002 | game | 2 / 164 | 1160 / 64208 | 1.81% | 0 |
| ov003 | game | 2 / 235 | 64 / 82696 | 0.08% | 0 |
| ov005 | game | 1 / 270 | 108 / 96684 | 0.11% | 0 |
| ov006 | game | 1 / 321 | 104 / 105588 | 0.10% | 0 |

Modules with nothing done: itcm, ov000, ov001, ov004.

## Started, not linked

Source files in `src/` that the build does not use yet (not marked `complete`).

| File | Functions | Bytes |
|---|---|---:|
| `src/game/main/menus/battle_setup_load_from_game.c` | BattleSetup_LoadFromGame | 1648 |
| `src/game/main/menus/campaign_map_init.c` | CampaignMap_Init | 3036 |
