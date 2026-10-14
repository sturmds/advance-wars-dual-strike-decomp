#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_ov004_022bf158.h"

/* Needed to match, with srcRow set inside the column loop: without both,
 * srcRow, destRow and the hoisted zero of screenBlock get other registers. */
#pragma opt_loop_invariants off

/* Copies a 33 x 25 tile window of the tile map, starting at the scroll
 * position, into BG screen blocks at dest. Rows wrap at 32; columns past
 * 32 go into the next screen block (+0x400 entries). */
void Ov004_CopyViewToBgMap(Unk_ov004_022bf158* state, u16* dest) {
  int row;
  int col;
  int viewY;
  int stride;
  int viewX;
  u32 tileX;
  int tileY;
  int rowOffset;
  u32 rowBase;
  int destRow;

  viewY = state->viewY;
  tileY = viewY >> 3;
  viewX = state->viewX;
  stride = state->blocksWide * 32;
  tileX = viewX >> 3;
  state->viewScrollX = viewX & 0x1ff;
  state->viewScrollY = viewY & 0xff;
  rowBase = stride * tileY + tileX;
  for (row = 0, rowOffset = 0; row < SCREEN_HEIGHT / TILE_PIXELS + 1; row++) {
    destRow = ((tileY + row) & 31) << 5;

    for (col = 0; col < SCREEN_WIDTH / TILE_PIXELS + 1; col++) {
      int screenBlock = 0;
      int x = tileX + col;
      int index;
      int srcRow;

      if (x & 32) {
        screenBlock += 0x400;
      }
      index = screenBlock + (x & 31);
      index += destRow;
      /* Needed to match: the u32 sum gives the original's operand order. */
      srcRow = (rowBase + rowOffset) * 2;
      dest[index] = ((u16*)(srcRow + (u32)state->tileMap))[col];
    }
    rowOffset += stride;
  }
}
