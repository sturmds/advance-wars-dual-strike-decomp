#ifndef GAME_MAP_DATA_H
#define GAME_MAP_DATA_H

#include "types.h"

/* The map being loaded, unpacked by Map_LoadData into data_0229eadc. */
typedef struct MapData {
  u8 width;
  u8 height;
  u16 tileCodes
      [1]; /* width * height graphic codes, row by row; Unk_02080440_48.tileCodeTerrain maps a code to its terrain */
} MapData;

/* The buffer Map_LoadData allocates for a built-in map: MapData and room
 * for its tile codes. Which largest map the 0xa14 bytes were sized for is
 * not known. */
typedef struct MapDataBuffer {
  u8 width;
  u8 height;
  u8 unk_02[0xa14 - 2];
} MapDataBuffer;

/* The buffer CombatSetup_LoadMapPreviews unpacks each Combat mode map
 * file into, 2 bytes smaller than MapDataBuffer. */
typedef struct CombatMapBuffer {
  u8 width;
  u8 height;
  u8 unk_02[0xa12 - 2];
} CombatMapBuffer;

extern MapData* data_0229eadc;

#endif
