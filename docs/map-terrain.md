# Map terrain: large objects and their tile codes

A survey of the map data unpacked by `make setup` (all 260 tile blocks), for the terrain ids that make up the large objects. The ids and names are in `enum TerrainId` (`include/game/terrain.h`); the names in quotes are what the terrain panel shows. Tile codes go through `/bmap/003` (`/bmap/002` for map 0xf8) to give a terrain id and an owner (`docs/maps.md`).

## How a large object is laid out

A 3x3 or 4x4 object has one centre tile with the object's terrain id. The other tiles of the middle row have codes of their own but terrain id 9 (`TERRAIN_OBJECT_PART`, "Sea" on the panel). The top and bottom rows are code 0x1a4, also terrain 9.

`Map_DrawSpecialTerrain` scans each map for the centre ids and adds a `MapObject` for each (`include/game/map_object.h`). `MapObject_GetKindOffset` moves the object's position from the centre to its top-left tile. The tile units attack (`MapObject_GetTargetTile`) is (x+1, y+2) from the top-left for kinds 3, 5 and 10, which is the bottom centre of a 3x3 object; the map data does not mark it. `MapPreview_DrawBlock` draws the object's picture on the terrain-9 tile below a Black Cannon or Deathray centre.

## Objects

| Object | Centre terrain id | Centre codes | Middle row codes | Size | `MapObject` kind | Maps |
|---|---|---|---|---|---|---|
| Minicannon | 21 | 0x182, 0x183, 0x184, 0x185 (one per facing) | - | 1x1 | 4 | 14 maps |
| Black Crystal ("Crystal") | 23 | 0x1a1 | - | 1x1 | 9 | 8 maps |
| Black Cannon ("Cannon") | 26 | 0x187, 0x18a | 0x186/0x188, 0x189/0x18b | 3x3 | 3 | Cannon Land (both codes), Crystal Calamity |
| Black Obelisk ("Obelisk") | 29 | 0x18d | 0x18c/0x18e | 3x3 | 10 | Crystal Calamity, For the Future! |
| Volcano | 28 | 0x1a7, 0x1a2, 0x1a3 | 0x1a6, 0x1a8, 0x1a9 | 4x4 | 2 | Cape Splinter, Ring of Fire (both maps) |
| Missile platform ("Blocked") | 31 | 0x1ab | 0x1aa, 0x1ac, 0x1ad | 4x4 | 8 | Dark City, Convoy Cape, Lone Wolf, Surrounded! |
| Fortress ("Blocked") | 31 | 0x1af | 0x1ae, 0x1b0, 0x1b1 | 4x4 | 8 | Silo Sweep, Omens and Signs (both maps), Surrounded!, Victory or Death! (0xfc) |
| Grand Bolt weak point ("G Bolt") | 24 | 0x12b, 0x137, 0x171 | - | 1x1 | 11-13 | Means to an End (0xf8) |

- On Cape Splinter the volcano's top row is code 0x1a5 instead of 0x1a4 (still terrain 9).
- The missile platform and the fortress share terrain id 31 and object kind 8 (`MAP_OBJECT_BLOCKED`); only their codes differ. Kind 8 is created with 0 HP and has no target tile, so `Map_GetObjectAt` never returns it. Its picture comes from `/bmap/0a5` or `0a6` (`MapHeader.extraObjTileFile`), or `0a7` when the game state's `unk_5c` callback returns 1 (`Map_LoadExtraObjTiles`). Which code sets that callback is not known yet.
- The Grand Bolt's body is 158 tiles of terrain 27 (`TERRAIN_GRAND_BOLT`). They use codes that `/bmap/003` gives to sea, shoal, river or no terrain, and that `/bmap/002` maps to terrain 27, which is why that map has its own table. Each weak point's owner bits pick its object kind (11 + owner). A destroyed weak point becomes terrain 27, code 0x17a (`MapObject_DestroyWeakPoint`). Two of the three weak points are 3 tiles above a base.
- The Minicannon's owner bits are its facing, not its owner: `Map_DrawSpecialTerrain` passes them to `Map_AddMinicannonObject`, and `MapObject_Draw` uses the variant to point the range cone. Watery Downs (0x0ad) has two `m0` cannons; Fortress Isle (0x0ae) has two of each code.

## When the weapons fire

`MapObjects_TickCountdowns` counts down kinds 1, 3, 4 and 5 (the Laser, see below, the Black Cannon, the Minicannon and the Deathray) on the turn of any player whose army colour is Black Hole (`ARMY_BLACK_HOLE`). The tile's owner bits play no part. On Watery Downs the cannons fire on Lash's turn because the map's header gives slot 2 army colour 5. The volcano counts down on player 1's turn instead.

`MapObject_Draw` does not draw the object: it marks the object's range for the B-button range display. Kind 1 marks its whole row and column, kind 3 a cone of size 10, kind 4 a cone of size 4 in the cannon's facing, and kind 5 three columns from 3 tiles below its top-left to the bottom of the map.

## Unused terrain

A scan of every map in `extracted/AWRE/assets/maps/` (2026-10-14) finds two terrain ids that no map uses, both Black Hole weapons from Advance Wars 2. The Design Room maps come from the save and were not scanned. Both were tried on Spann Island (0x08b) in a mod build.

| | Laser | Deathray |
|---|---|---|
| Terrain id | 25 | 30 |
| Codes | 0x181 only, owner 0 | centre 0x193, middle row 0x192/0x194, 3x3 |
| Terrain panel | name and description present | "Deathray", "This mighty laser cannon fires every 7 days.", the DO NOT ENTER picture |
| `MapObject` | none: `Map_DrawSpecialTerrain` has no case for terrain 25 | kind 5 (`Map_AddDeathrayObject`): 99 HP, state and startState 7, `unk_07` 80 |
| Map picture | the tile flickers between two broken animations | none: `MapObjects_QueueSpritesInRect` has no case for kind 5 |
| Countdown | - | on Black Hole turns, as above |
| Fires | never | never: `MapObjectTurnTask_TargetNext` acts only for the volcano, Black Cannon and Minicannon. Tested to day 8 |
| B-button range | none | three columns down to the bottom of the map |
| Attackable | no | no: `MapObject_GetTargetTile` gives it a target tile, but `TargetKind_GetChartColumn` has no case for kind 5 and returns column 0, so no weapon is picked and `RangeMap_MarkAttackTargets` never marks it |

Kind 1 is probably the Laser's object. No `Map_Add*Object` function creates it, but the code still handles it: it counts down on Black Hole turns, `TargetKind_GetChartColumn` gives it `DAMAGE_COLUMN_OBJECT_A`, `MapObject_GetTargetTile` gives it a 1x1 target, and its range is its row and column, the Laser's pattern in Advance Wars 2. Nothing ties it to terrain 25 yet.

Other codes in `/bmap/003` that no map uses:

- 0x18f-0x191: terrain 9, 27, 9, the middle row of an unknown 3x3 object whose centre is terrain 27 (`TERRAIN_GRAND_BOLT`).
- 0x1c0, a neutral HQ, and the Lab codes 0x1d9, 0x1da and 0x1dd (owners 0, 1 and 4).
- 0x1e0-0x1fd: a second set of HQ, base, city, airport, port and Lab codes for owners 0-4, laid out like 0x1c0-0x1dd. What tells the two sets apart is not known.
- 24 river and 3 sea shape codes.
