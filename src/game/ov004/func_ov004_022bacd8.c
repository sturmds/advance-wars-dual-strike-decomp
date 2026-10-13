#include "types.h"
#include "game/Unk_ov004_022bf158.h"

/* Copies a 33 x 25 tile window of the tile map, starting at the scroll
 * position, into BG screen blocks at dest. Rows wrap at 32; columns past
 * 32 go into the next screen block (+0x400 entries). Not matched yet
 * (94.8%): the original gives srcRow, the hoisted screenBlock zero and the
 * destination row offset other registers. Needed for the registers so far:
 * the destination row offset is kept in viewX, unused by then. */
void func_ov004_022bacd8(Unk_ov004_022bf158* state, u16* dest) {
  int row;
  int col;
  int viewY;
  int stride;
  int viewX;
  u32 tileX;
  int tileY;

  viewY = state->viewY;
  tileY = viewY >> 3;
  viewX = state->viewX;
  stride = state->blocksWide * 32;
  tileX = viewX >> 3;
  state->viewScrollX = viewX & 0x1ff;
  state->viewScrollY = viewY & 0xff;
  for (row = 0; row < 25; row++) {
    int srcRow;

    viewX = (tileY + row) & 31;
    viewX <<= 5;
    srcRow = stride * tileY + tileX + row * stride;

    for (col = 0; col < 33; col++) {
      int screenBlock;
      int x;

      screenBlock = 0;
      x = tileX + col;

      if (x & 32) {
        screenBlock += 0x400;
      }
      dest[screenBlock + (x & 31) + viewX] =
          ((u16*)(srcRow * 2 + (u8*)state->tileMap))[col];
    }
  }
}
