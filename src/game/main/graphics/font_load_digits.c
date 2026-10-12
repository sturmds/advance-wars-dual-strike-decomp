#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;
extern u8 gDigitFontTiles[]; /* digit font: 40 tiles, 4bpp */

void Font_SetDigitEndTile(
    int tile); /* stores tile in data_027c0280->fontState->digitEndTile */
void Mem_CopyAuto(const void* src, void* dst,
                  u32 size); /* copies in 32-bit units when size allows */

/* Copies the digit font (two colours, each digit split over two tiles)
 * to tiles baseTile + 0x36 to baseTile + 0x5d of the layer 0 tile data,
 * and passes the tile after them to Font_SetDigitEndTile. */
void Font_LoadDigits(int baseTile) {
  Font_SetDigitEndTile(baseTile + 0x5e);
  Mem_CopyAuto(gDigitFontTiles, data_027c0280->unk_e4 + (baseTile + 0x36) * 32,
               0x500);
}
