#ifndef GAME_TERRAIN_H
#define GAME_TERRAIN_H

#include "types.h"
#include "game/text_ref.h"

/* Terrain ids, from romhacking.net doc 890 (RadioTails) and the names the
 * terrain panel shows (SecondScreen_DrawTerrainDetails). A map tile code
 * gives its terrain id through a table from /bmap (bits 0-4; bits 5-7 are
 * the owner). docs/map-terrain.md lists the codes of the large objects. */
enum TerrainId {
  TERRAIN_NONE,
  TERRAIN_PLAIN,
  TERRAIN_RIVER,
  TERRAIN_MOUNTAIN,
  TERRAIN_WOOD,
  TERRAIN_ROAD,
  TERRAIN_CITY,
  TERRAIN_SEA,
  TERRAIN_HQ,
  TERRAIN_OBJECT_PART =
      9, /* the other tiles of a 3x3 or 4x4 object; the panel calls it "Sea" */
  TERRAIN_AIRPORT = 10,
  TERRAIN_PORT = 11,
  TERRAIN_BRIDGE = 12,
  TERRAIN_SHOAL = 13,
  TERRAIN_BASE = 14,
  TERRAIN_PIPE = 15,
  TERRAIN_PIPE_SEAM = 16,
  TERRAIN_SILO = 17,
  TERRAIN_SILO_USED = 18,
  TERRAIN_REEF = 19,
  TERRAIN_LAB = 20,
  TERRAIN_MINICANNON = 21,
  TERRAIN_COM_TOWER = 22,
  TERRAIN_BLACK_CRYSTAL = 23,
  TERRAIN_GRAND_BOLT_WEAK_POINT =
      24,             /* "G Bolt": one of the three tiles units attack */
  TERRAIN_LASER = 25, /* no map uses it */
  TERRAIN_BLACK_CANNON = 26,
  TERRAIN_GRAND_BOLT = 27, /* "G Bolt": the body, and a destroyed weak point */
  TERRAIN_VOLCANO = 28,
  TERRAIN_BLACK_OBELISK = 29,
  TERRAIN_DEATHRAY = 30, /* no map uses it */
  TERRAIN_BLOCKED = 31   /* centre of a 4x4 missile platform or fortress */
};

/* Entries in a table with one value per terrain id: Ov000_gTerrainStats,
 * a row of a movement cost chart (CoPowerBlock.movementCost,
 * Ov000_gCost*Movement). */
#define TERRAIN_ID_COUNT 32

/* Fixed stats of one terrain id, 0x18 bytes: Ov000_gTerrainStats holds one
 * per terrain id, and Ov000_gUnitStats starts right after it. Terrain 0 is
 * all zero. */
typedef struct TerrainStats {
  /* packed tiles of the terrain panel's picture (Data_Unpack); NULL makes
   * the panel take one from data_ov000_022f65c0 */
  void* picture;
  void* palette;       /* 16 colours for the picture */
  u32 unk_08;          /* 0 in every entry */
  TextRef name;        /* group 0xc000, the name the terrain panel shows */
  TextRef description; /* group 0xc000 */
  s32 defenseStars;    /* plain 1, wood 2, city 3, mountain and HQ 4 */
} TerrainStats;

typedef char TerrainStats_size_check[sizeof(TerrainStats) == 0x18 ? 1 : -1];

extern TerrainStats Ov000_gTerrainStats[TERRAIN_ID_COUNT];

/* One entry of data_02166758 per terrain id: Terrain_GetIconTiles reads
 * tiles, Terrain_GetPalette reads palette (through data_02166759). */
typedef struct TerrainIcon {
  u8 tiles;   /* icon number: 256 bytes of tile data each */
  u8 palette; /* 32 bytes of palette each */
} TerrainIcon;

typedef char TerrainIcon_size_check[sizeof(TerrainIcon) == 2 ? 1 : -1];

#endif
