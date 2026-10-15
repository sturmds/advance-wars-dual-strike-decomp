#include "types.h"
#include "game/Unk_02080440.h"

#define TILE_OWNER(x, y) \
  MAP_TILE_OWNER(        \
      MAP_STATE->mapTiles->tiles[(x) + MAP_STATE->mapTiles->rowStart[y]])

/* TRUE when the tile at x 14, y 1 belongs to player 1 or 2. Which map this is for is unknown. */
BOOL func_ov001_02351640(void) {
  int owner = TILE_OWNER(14, 1);

  if (owner == 1) {
    return TRUE;
  }
  return owner == 2;
}
