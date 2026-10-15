#include "types.h"
#include "game/oam_cells.h"
#include "game/math.h"

void Oam_PutSprite(int x, int y, void* shape, int tileAndPalette);

/* Draws value (at most 999) as digit sprites, ones digit first, each digit 8
 * pixels left of the last, starting at x - 8. Digit tiles sit 2 apart from
 * tile + 0x1000. */
void NumberSprite_PutDigits(int x, int y, u32 value, int tile) {
  u8 digitY;

  if (value > 999) {
    value = 999;
  }
  x = (u16)(x - 8);
  digitY = y;
  for (;;) {
    Oam_PutSprite(x & 0x1ff, digitY, &data_027c0038,
                  tile + HwMod32(value, 10) * 2 + 0x1000);
    value = (u16)HwDiv32(value, 10);
    if (value == 0) {
      break;
    }
    x = (u16)(x - 8);
  }
}
