#include "types.h"

extern u8 data_02166310[];

void Oam_PutSprite(int x, int y, void* shape, int tileAndPalette);

/* Draws value in decimal as one sprite per digit (digit tiles start at 16).
 * x is the ones digit; each further digit sits 7 pixels to the left. */
void Oam_PutSmallNumber(u16 x, int y, u32 value) {
  for (;;) {
    Oam_PutSprite(x & 0x1ff, y & 0xff, data_02166310, value % 10 + 16);
    value /= 10;
    if (value == 0) {
      break;
    }
    x -= 7;
  }
}
