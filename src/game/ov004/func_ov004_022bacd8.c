#include "types.h"
#include "game/Unk_ov004_022bf158.h"

/* Copies a 33 x 25 tile window of the tile map, starting at the scroll
 * position, into BG screen blocks at dest. Rows wrap at 32; columns past
 * 32 go into the next screen block (+0x400 entries). Not matched yet
 * (94.4%): the original finishes destRow before srcRow, and destRow and the
 * row * stride step swap registers. The declaration order came from a sweep
 * script. */
void func_ov004_022bacd8(Unk_ov004_022bf158* state, u16* dest) {
  int row;
  int viewY;
  int stride;
  int viewX;
  int col;
  u32 tileX;
  int srcRowStart;
  int tileY;

  viewY = state->viewY;
  tileY = viewY >> 3;
  viewX = state->viewX;
  stride = state->blocksWide * 32;
  srcRowStart = stride * tileY;
  tileX = viewX >> 3;
  state->viewScrollX = viewX & 0x1ff;
  state->viewScrollY = viewY & 0xff;
  srcRowStart += tileX;
  for (row = 0; row < 25; row++) {
    int destRow = ((tileY + row) & 31) << 5;
    int srcRow = (srcRowStart + row * stride) * 2;

    for (col = 0; col < 33; col++) {
      int screenBlock = 0;
      int x = tileX + col;

      if (x & 32) {
        screenBlock += 0x400;
      }
      dest[screenBlock + (x & 31) + destRow] =
          ((u16*)(srcRow + (u8*)state->tileMap))[col];
    }
  }
}
