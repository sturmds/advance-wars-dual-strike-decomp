#ifndef GAME_AI_REACH_TILE_H
#define GAME_AI_REACH_TILE_H

#include "types.h"

/* One entry of a list of tiles the acting unit can reach, built by the AI
 * from Unk_02080440_48.rangeMap. The list ends with an entry whose cost
 * is -1. */
typedef struct AiReachTile {
  u8 x;
  u8 y;
  s16 cost; /* the range map value of the tile */
} AiReachTile;

#endif
