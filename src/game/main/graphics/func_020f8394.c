#include "types.h"

void Palette_Load(const void* src, u32 offset, u32 size);

/* palette of the number font */
const u8 data_02138c68[32] = {
#include "data/main/data_02138c68.inc.c"
};
static const u16 data_02138c88[64] = {
#include "data/main/data_02138c88.inc.c"
};
/* digit font: 40 tiles, 4bpp */
const u8 gDigitFontTiles[1280] = {
#include "data/main/gDigitFontTiles.inc.c"
};
extern u16 data_02139308[];

void func_020f8394(int value) {
  int index = (value >> 2) & 15;

  Palette_Load(&data_02138c88[index], 0x238, 2);
  Palette_Load(&data_02139308[index], 0x228, 2);
}
