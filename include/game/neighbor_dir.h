#ifndef GAME_NEIGHBOR_DIR_H
#define GAME_NEIGHBOR_DIR_H

#include "types.h"

/* One row per direction 0-3 (data_02166378) for the four tiles next to a
 * center tile. mask is the direction's bit in a drop-direction mask. */
typedef struct NeighborDir {
  u8 mask;
  s8 dx;
  s8 dy;
  s8 pathStep[2];
} NeighborDir;

extern NeighborDir data_02166378[4];

#endif
