#ifndef GAME_TILE_OFFSET_H
#define GAME_TILE_OFFSET_H

#include "types.h"

#define TILE_OFFSET_END 127 /* dx that ends an offset list */

/* One tile offset of a list Map_AddTileOffset walks. */
typedef struct TileOffset {
  s16 dx;
  s16 dy;
} TileOffset;

/* The offset lists by list number; entry 0 is NULL. */
extern TileOffset* data_0216638c[];
extern TileOffset data_02166720[];
extern TileOffset data_021669b0[];
extern TileOffset data_02166c08[];

#endif
