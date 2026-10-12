#ifndef GAME_TILE_POINT_H
#define GAME_TILE_POINT_H

#include "types.h"

/* A map tile position. */
typedef struct TilePoint {
  s16 x;
  s16 y;
} TilePoint;

/* A map tile position stored as u16; lists of these end at x == 0xffff. */
typedef struct TilePointU16 {
  u16 x;
  u16 y;
} TilePointU16;

#endif
