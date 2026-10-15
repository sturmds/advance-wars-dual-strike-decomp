#include "types.h"
#include "game/Unk_02080440.h"

/* Nonzero when player 1 owns the tile at x 9, y 1. */
BOOL Ov001_IsTileOwnedByPlayer1_x9y1(void) {
  MapTiles* tiles = MAP_STATE->mapTiles;

  return MAP_TILE_OWNER(tiles->tiles[tiles->rowStart[1] + 9]) == 1;
}
