#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280 *data_027c0280;
extern u8 gDigitFontTiles[];      /* digit font: 40 tiles, 4bpp */

void func_020213c0(int tile);    /* stores tile in data_027c0280->unk_14 and sets a flag there */
void func_02003c6c(const void *src, void *dst, u32 size);    /* copies in 32-bit units when size allows */

/* Copies the digit font (two colours, each digit split over two tiles)
 * to tiles baseTile + 0x36 to baseTile + 0x5d of the layer 0 tile data,
 * and passes the tile after them to func_020213c0. */
void Font_LoadDigits(int baseTile)
{
    func_020213c0(baseTile + 0x5e);
    func_02003c6c(gDigitFontTiles, data_027c0280->unk_e4 + (baseTile + 0x36) * 32, 0x500);
}
