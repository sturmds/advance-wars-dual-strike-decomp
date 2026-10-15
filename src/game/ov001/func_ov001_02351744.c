#include "types.h"
#include "game/Unk_02080440.h"

#define TILE_OWNER(x, y) \
  MAP_TILE_OWNER(        \
      MAP_STATE->mapTiles->tiles[(x) + MAP_STATE->mapTiles->rowStart[y]])

/* TRUE when the tiles at (8, 2) and (8, 15) both belong to player 1. Which map this is for is unknown. */
BOOL func_ov001_02351744(void) {
  if (TILE_OWNER(8, 2) != 1) {
    return FALSE;
  }
  return TILE_OWNER(8, 15) == 1;
}
