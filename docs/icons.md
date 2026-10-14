# Icons

Icons are small OBJ (sprite) pictures that the menus load by id with `Icon_Load`, `Icon_LoadTo` and draw with `Icon_Draw` (`src/game/main/icon/`, `include/game/icon.h`). This page lists the runs of icons the main menus load in loops, and how the names in `enum IconId` were found.

## Where the pictures are

- 336 icon ids in six banks. `Icon_GetBank` picks the bank from the id: 0-74, 75-88, 89, 90-101, 102-275, 276-335.
- Each bank has one file, `/icon/res_icon0_cg` to `res_icon5_cg`. `File_LocalizePath` adds a language suffix; the US ROM reads `res_iconN_cg_E`. The tiles are not compressed: 4 bits per pixel, 32 bytes per 8x8 tile, the icons of a bank one after another (`Icon_GetFileOffset`).
- `gIconSize[id]` gives the width and height in tiles. The tiles of an icon go row by row.
- `gIconBankInfo[bank].palette` points to the bank's 16 colours in main (0x02139208 + 0x20 * bank). Every screen loads the bank's own palette (`IconBank_LoadPalette`), so the colours below are the ones the game shows.

The tile counts of banks 0, 1, 3, 4 and 5 add up to the size of their `_E` files exactly (bank 4: 21504 bytes), which checks the sizes and the order. Bank 2 holds icon 89, which has size 0 by 0.

To look at them, decode each `_E` file with `gIconSize` and the bank palette into a PNG (Python's `zlib` and `struct` are enough). Keep the output in `work/` or `build/`, never in git.

## Runs

A run holds one icon per value: the run's first id plus the value gives the icon. The table gives the first id, the count, what the pictures show and where the code indexes the run.

| Name | Ids | Count | Picture | Evidence |
|---|---|---|---|---|
| `ICON_ARMY_BANNERS` | 75-79 | `ARMY_BLACK_HOLE` | Army emblem with a dog, 4x2 tiles, red, blue, green, yellow, grey | Status screen and CO EXP screen load `ICON_ARMY_BANNERS - 1 + armyColour` |
| `ICON_ARMY_EMBLEMS` | 80-84 | `ARMY_BLACK_HOLE` | The emblem alone, 2x2 | `NextTurn_DrawArmyName` draws `ICON_ARMY_EMBLEMS - 1 + army`; `PlayerInfo_DrawCoIcons` picks tile `12 + (army - 1) * 4`, where `PlayerInfo_Init` loads the run |
| `ICON_DIGITS_OUTLINED` | 108-117 | `ICON_DIGIT_COUNT` | 0-9, white with a black outline, 1x2 | Force Rank and map select screens, at tile base + 2 * digit |
| `ICON_DIGITS_GREY` | 120-129 | `ICON_DIGIT_COUNT` | 0-9, grey, 1x2 | Force Rank screen, next to `ICON_DIGITS_OUTLINED`; a guess: the grey set shows what is not available |
| `ICON_DIGITS` | 140-149 | `ICON_DIGIT_COUNT` | 0-9, white, 1x2 | Named earlier; CO select, CO rank, CO EXP, Force Rank, player info, result stats |
| `ICON_DIGITS_SMALL` | 150-159 | `ICON_DIGIT_COUNT` | 0-9, yellow, one tile | Map select, history, player info, CO rank, CO EXP; ov005's shop and sound room page numbers draw `150 + digit` with `162` ("/") between |
| `ICON_PLACES` | 188-192 | `ICON_PLACE_COUNT` | "1st" to "5th" | Map select loads `WAR_ROOM_PLACE_COUNT` of them beside the War Room records; total rank loads `CAMPAIGN_TOP_CO_COUNT` beside the top COs |
| `ICON_PLAYER_LABELS` | 204-207 | 4 | "P1" to "P4" | Vs setup, followed by `ICON_CPU_LABEL` (208, "CPU") |
| `ICON_STAT_GAUGE` | 225-265 | 41 | A 3x1 bar, from empty to full in two colours | `PlayerInfo_Init` loads it at BG tile 716 + 3 * step; `player_info_draw_unit_stats.c` draws `GAUGE_TILE(step)` = 0x308 + 3 * step, so steps -20 to +20 around icon 245 |
| `ICON_DIGITS_SMALL_WHITE` | 266-275 | `ICON_DIGIT_COUNT` | 0-9, white, one tile | Map select loads it at OBJ tile 0x68 of the other screen; `MapSelect_Draw` draws `ones * 2 + 104` |
| `ICON_TEAMS` | 276-279 | `ICON_TEAM_COUNT` | "A TEAM" to "D TEAM", 8x4 | Vs setup |
| `ICON_SETTING_LABELS` | 280-288 | `ICON_SETTING_COUNT` | "Fog", "Weather", "Land", "Funds", "Turn", "Capt", "Power", "Visuals", "CPU AI" | `VsSetup_DrawSettingRow` draws tile `row * 8 + 180`, where the run is loaded |
| `ICON_ON`, `ICON_OFF` | 289, 290 | | "ON", "OFF" | `TILE_ON` and `TILE_OFF` of `VsSetup_DrawSettingRow` |
| `ICON_VISUAL_TYPES` | 291-294 | 4 | "TYPE A" to "TYPE D" | Setting row 7, by `VISUAL_A` to `VISUAL_D` |
| `ICON_SETTING_CIRCLES` | 295-303 | 9 | A coloured disc, 4x4 | Drawn behind each setting row (tile `row * 16 + 300`) |
| `ICON_WEATHERS` | 304-308 | 5 | Two arrows in a circle, sun, snowman, rain cloud, a cloud with a tail | Setting row 1. Vs setup loads 304, 305, 307, 306, 308 for the setting's values 0-4 |
| `ICON_TILESETS` | 309-312 | 4 | Blocks of ground: plain, snowy with a snowman, plain, striped | Setting row 2 ("Land"), by `setup.unk_2dd`, the map tileset |
| `ICON_AI_KINDS` | 313-316 | 4 | Square buttons: a jagged edge, an arrow, a face, an arrow into a bar | Setting row 8 ("CPU AI"), by `setup.unk_2e2`; what each one means is a guess |
| `ICON_DIGITS_SETUP` | 317-326 | `ICON_DIGIT_COUNT` | 0-9, white, 1x2, in bank 5 | `TILE_DIGIT(d)` of `VsSetup_DrawSettingRow` (funds, turn and capture limits) |
| `ICON_TEAM_LETTERS` | 327-330 | 4 | "a" to "d" in the team colours | Vs setup |
| `ICON_TEAM_LETTERS_GREY` | 331-334 | 4 | "a" to "d", grey | Vs setup |

So the runs 150-159, 108-117, 120-129 and 0x10a (266-275) are digit sets, as the question about `ICON_DIGITS` guessed, each in its own colour or size. Icons 163-172 are one more set (small dark digits) that no main code loads in a loop.

Single icons next to the runs: 139 "RANK" (`ICON_RANK_LABEL`), 162 "/" (`ICON_SMALL_SLASH`), 200 "DAY" and 201 "PTS" (`ICON_DAY_LABEL`, `ICON_PTS_LABEL`), 174 and 175 the move and range marks, 176-180 "-2", "-1", "+1", "+2", "+3" (`player_info_draw_unit_stats.c`), 184-187 the War Room EXP bonus on the CO EXP screen ("NO BONUS", "EXP x1.5", "x2", "x2.5"; 183 in the other modes), 213 a crown.
