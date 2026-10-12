#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Nonzero when player 1 or player 2 owns the tile at x 17, y 1. */
BOOL Ov001_IsTileOwnedByPlayer1Or2_x17y1(void) {
  MapTiles* tiles = STATE->mapTiles;
  int owner = MAP_TILE_OWNER(tiles->tiles[tiles->rowStart[1] + 17]);

  if (owner == 1) {
    return TRUE;
  }
  return owner == 2;
}
