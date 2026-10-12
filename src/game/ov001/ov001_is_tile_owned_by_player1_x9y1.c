#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Nonzero when player 1 owns the tile at x 9, y 1. */
BOOL Ov001_IsTileOwnedByPlayer1_x9y1(void) {
  MapTiles* tiles = STATE->mapTiles;

  return MAP_TILE_OWNER(tiles->tiles[tiles->rowStart[1] + 9]) == 1;
}
