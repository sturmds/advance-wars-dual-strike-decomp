#include "types.h"
#include "game/Unk_ov004_022bf158.h"

/* Fills the 32x32 block at (x, y) of the tile map with one tile value. */
void Ov004_FillBlock(Unk_ov004_022bf158* state, int x, int y, u16 tile) {
  int offset;
  int i;
  int stride;
  int j;
  u16* map;

  stride = state->blocksWide * 32;
  map = (u16*)state->tileMap;
  offset = y * 32 * stride + x * 32;

  for (i = 0; i < 32; i++) {
    for (j = 0; j < 32; j++) {
      (map + offset + i * stride)[j] = tile;
    }
  }
}
