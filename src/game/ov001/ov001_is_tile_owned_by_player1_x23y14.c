#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Nonzero when player 1 owns the tile at x 23, y 14. */
BOOL Ov001_IsTileOwnedByPlayer1_x23y14(void) {
  MapTiles* tiles = STATE->mapTiles;

  return MAP_TILE_OWNER(tiles->tiles[tiles->rowStart[14] + 23]) == 1;
}
