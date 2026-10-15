#include "types.h"
#include "game/Unk_02080440.h"

#define TILE_OWNER(x, y) \
  MAP_TILE_OWNER(        \
      MAP_STATE->mapTiles->tiles[(x) + MAP_STATE->mapTiles->rowStart[y]])

/* TRUE when the tiles at (7, 7), (7, 12), (12, 7) and (12, 12) all belong to player 1. Which map this is for is unknown. */
BOOL func_ov001_02351804(void) {
  if (TILE_OWNER(7, 7) != 1) {
    return FALSE;
  }
  if (TILE_OWNER(7, 12) != 1) {
    return FALSE;
  }
  if (TILE_OWNER(12, 7) != 1) {
    return FALSE;
  }
  return TILE_OWNER(12, 12) == 1;
}
