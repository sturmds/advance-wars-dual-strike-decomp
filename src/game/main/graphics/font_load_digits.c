#include "types.h"
#include "game/Unk_02088d04.h"

extern u8 gDigitFontTiles[]; /* digit font: 40 tiles, 4bpp */
#define DIGIT_FONT_TILES 40
#define DIGIT_FONT_FIRST_TILE 0x36 /* after baseTile */

void Font_SetDigitEndTile(
    int tile); /* stores tile in data_027c0280->fontState->digitEndTile */
void Mem_CopyAuto(const void* src, void* dst,
                  u32 size); /* copies in 32-bit units when size allows */

/* Copies the digit font (two colours, each digit split over two tiles)
 * to tiles baseTile + 0x36 to baseTile + 0x5d of the layer 0 tile data,
 * and passes the tile after them to Font_SetDigitEndTile. */
void Font_LoadDigits(int baseTile) {
  Font_SetDigitEndTile(baseTile + DIGIT_FONT_FIRST_TILE + DIGIT_FONT_TILES);
  Mem_CopyAuto(gDigitFontTiles,
               data_027c0280->unk_e4 +
                   (baseTile + DIGIT_FONT_FIRST_TILE) * TILE_4BPP_SIZE,
               DIGIT_FONT_TILES * TILE_4BPP_SIZE);
}
