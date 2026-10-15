#include "types.h"
#include "game/Unk_02080440.h"

/* Nonzero when player 1 or player 2 owns the tile at x 17, y 1. */
BOOL Ov001_IsTileOwnedByPlayer1Or2_x17y1(void) {
  MapTiles* tiles = MAP_STATE->mapTiles;
  int owner = MAP_TILE_OWNER(tiles->tiles[tiles->rowStart[1] + 17]);

  if (owner == 1) {
    return TRUE;
  }
  return owner == 2;
}
