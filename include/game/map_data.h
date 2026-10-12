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

extern MapData* data_0229eadc;

#endif
