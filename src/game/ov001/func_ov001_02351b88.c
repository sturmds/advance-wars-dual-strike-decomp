#include "types.h"
#include "game/Unk_02080440.h"

#define TILE_OWNER(x, y) \
  MAP_TILE_OWNER(        \
      MAP_STATE->mapTiles->tiles[(x) + MAP_STATE->mapTiles->rowStart[y]])

/* FALSE when player 3 owns any of the tiles (1, 4), (16, 1), (4, 16) or (19, 13). Which map this is for is unknown. */
BOOL func_ov001_02351b88(void) {
  if (TILE_OWNER(1, 4) == 3) {
    return FALSE;
  }
  if (TILE_OWNER(16, 1) == 3) {
    return FALSE;
  }
  if (TILE_OWNER(4, 16) == 3) {
    return FALSE;
  }
  return TILE_OWNER(19, 13) != 3;
}
