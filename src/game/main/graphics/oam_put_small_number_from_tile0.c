#include "types.h"

extern u8 data_02166310[];

void Oam_PutSprite(int x, int y, void* shape, int tileAndPalette);

/* Like Oam_PutSmallNumber, but the digit tiles start at 0 instead of 16. */
void Oam_PutSmallNumberFromTile0(u16 x, int y, u32 value) {
  for (;;) {
    Oam_PutSprite(x & 0x1ff, y & 0xff, data_02166310, value % 10);
    value /= 10;
    if (value == 0) {
      break;
    }
    x -= 7;
  }
}
