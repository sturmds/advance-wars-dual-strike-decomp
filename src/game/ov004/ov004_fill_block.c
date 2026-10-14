#include "types.h"
#include "game/Unk_ov004_022bf158.h"

/* Fills the 32x32 block at (x, y) of the tile map with one tile value. */
void Ov004_FillBlock(Unk_ov004_022bf158* state, int x, int y, u16 tile) {
  int offset;
  int i;
  int stride;
  int j;
  u16* map;

  stride = state->blocksWide * OV004_BLOCK_TILES;
  map = (u16*)state->tileMap;
  offset = y * OV004_BLOCK_TILES * stride + x * OV004_BLOCK_TILES;

  for (i = 0; i < OV004_BLOCK_TILES; i++) {
    for (j = 0; j < OV004_BLOCK_TILES; j++) {
      (map + offset + i * stride)[j] = tile;
    }
  }
}
