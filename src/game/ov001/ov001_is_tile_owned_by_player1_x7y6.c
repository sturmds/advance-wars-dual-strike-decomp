#include "types.h"
#include "game/Unk_02080440.h"

/* Nonzero when player 1 owns the tile at x 7, y 6. */
BOOL Ov001_IsTileOwnedByPlayer1_x7y6(void) {
  MapTiles* tiles = MAP_STATE->mapTiles;

  return MAP_TILE_OWNER(tiles->tiles[tiles->rowStart[6] + 7]) == 1;
}
