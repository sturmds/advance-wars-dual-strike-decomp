#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Fills MapTiles.rowStart: the index in the tile arrays of each row's first
 * tile, row * map width. */
void Map_BuildRowStarts(void) {
  u8 row;

  for (row = 0; row < STATE->unk_1c4->mapHeight; row++) {
    STATE->mapTiles->rowStart[row] = row * STATE->unk_1c4->mapWidth;
  }
}
