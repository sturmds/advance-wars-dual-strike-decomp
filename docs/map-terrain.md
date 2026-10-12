# Map terrain: large objects and their tile codes

A survey of the map data unpacked by `make setup` (all 260 tile blocks), for the terrain ids that make up the large objects. The ids and names are in `enum TerrainId` (`include/game/terrain.h`); the names in quotes are what the terrain panel shows. Tile codes go through `/bmap/003` (`/bmap/002` for map 0xf8) to give a terrain id and an owner (`docs/maps.md`).

## How a large object is laid out

A 3x3 or 4x4 object has one centre tile with the object's terrain id. The other tiles of the middle row have codes of their own but terrain id 9 (`TERRAIN_OBJECT_PART`, "Sea" on the panel). The top and bottom rows are code 0x1a4, also terrain 9.

`Map_DrawSpecialTerrain` scans each map for the centre ids and adds a `MapObject` for each (`include/game/map_object.h`). `MapObject_GetKindOffset` moves the object's position from the centre to its top-left tile. The tile units attack (`MapObject_GetTargetTile`) is (x+1, y+2) from the top-left for kinds 3, 5 and 10, which is the bottom centre of a 3x3 object; the map data does not mark it. `MapPreview_DrawBlock` draws the object's picture on the terrain-9 tile below a Black Cannon or Black Obelisk centre.

## Objects

| Object | Centre terrain id | Centre codes | Middle row codes | Size | `MapObject` kind | Maps |
|---|---|---|---|---|---|---|
| Minicannon | 21 | 0x182, 0x183, 0x184, 0x185 (one per facing, probably) | - | 1x1 | 4 | 14 maps |
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
- `/bmap/003` also has codes no map uses: 0x18f-0x191 (terrain 9, 27, 9: the middle row of an object, probably 3x3), a 3x3 Deathray with centre 0x193 (terrain 30, `MAP_OBJECT_DEATHRAY`, middle row 0x192/0x194), and 0x181 (terrain 25, "Laser").
