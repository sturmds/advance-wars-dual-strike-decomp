#include "types.h"
#include "game/Unk_02080440.h"

/* Nonzero when player 1 owns the tile at x 23, y 14. */
BOOL Ov001_IsTileOwnedByPlayer1_x23y14(void) {
  MapTiles* tiles = MAP_STATE->mapTiles;

  return MAP_TILE_OWNER(tiles->tiles[tiles->rowStart[14] + 23]) == 1;
}
