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

/* Fixed stats of one terrain id, 0x18 bytes, in ov000 at 0x022f4ccc.
 * The table holds terrain ids 0-30; Ov000_gUnitStats starts right
 * after. unk_10 and unk_14 are the name and description of the next
 * terrain id: the entries really start 8 bytes earlier (issue 0064). */
typedef struct TerrainStats {
  s32 defenseStars; /* plain 1, wood 2, city 3, mountain and HQ 4 */
  void* unk_04;
  void* unk_08;
  u32 unk_0c;
  TextRef unk_10; /* group 0xc000; likely the name */
  TextRef unk_14; /* group 0xc000 */
} TerrainStats;

typedef char TerrainStats_size_check[sizeof(TerrainStats) == 0x18 ? 1 : -1];

extern TerrainStats Ov000_gTerrainStats[];

#endif
