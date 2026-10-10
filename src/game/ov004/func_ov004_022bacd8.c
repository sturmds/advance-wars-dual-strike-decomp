#include "types.h"
#include "game/Unk_ov004_022bf158.h"

/* Copies a 33 x 25 tile window of the tile map, starting at the scroll
 * position, into BG screen blocks at dest. Rows wrap at 32; columns past
 * 32 go into the next screen block (+0x400 entries). */
void func_ov004_022bacd8(Unk_ov004_022bf158 *state, u16 *dest) {
    int tileY = state->viewY >> 3;
    int stride = state->blocksWide * 32;
    int srcRowStart = stride * tileY;
    int tileX;
    int rowOffset = 0;
    int row;
    int col;

    tileX = state->viewX >> 3;
    state->viewScrollX = state->viewX & 0x1ff;
    state->viewScrollY = state->viewY & 0xff;
    srcRowStart += tileX;
    for (row = 0; row < 25; row++) {
        int srcRow = srcRowStart + rowOffset;
        int destRow = ((tileY + row) & 31) << 5;

        for (col = 0; col < 33; col++) {
            int x = tileX + col;
            int screenBlock = 0;

            if (x & 32) {
                screenBlock = 0x400;
            }
            dest[destRow + screenBlock + (x & 31)] = ((u16 *)state->tileMap + srcRow)[col];
        }
        rowOffset += stride;
    }
}
