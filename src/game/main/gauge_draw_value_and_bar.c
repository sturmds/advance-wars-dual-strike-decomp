#include "types.h"
#include "game/oam_cells.h"

s32 HwDiv32(s32 numer, s32 denom);
s32 HwMod32(s32 numer, s32 denom);
void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);
void Gauge_QueueDigitPopIn(int x, int y, int digit, int frame, int matrixIndex);

#define BAR_CAP 5    /* points in the left cap */
#define BAR_CELL 8   /* points in one cell */
#define BAR_CELLS 15 /* cells between the caps */

/* Queues the sprites of a value gauge at (x, y): a label whose palette steps up
 * at 60, 80 and 100, the value as three pop-in digits (each starting 4 frames
 * after the one before), and a bar to the right. The bar has a left cap that
 * fills from 0 to 5, 15 cells of 8 each (5 to 125) and a right cap that fills
 * from 126 to 130. Callers pass frame -1 to draw the digits without the
 * pop-in. */
void Gauge_DrawValueAndBar(int x, int y, int value, int frame) {
  int palette;
  int digit;
  int rest;
  int i;
  u8 barY;

  if (y >= 192) {
    return;
  }
  if (value >= 100) {
    palette = 3;
  } else if (value >= 80) {
    palette = 2;
  } else if (value >= 60) {
    palette = 1;
  } else {
    palette = 0;
  }
  Sprite_QueueAt(6, x & 0x1ff, y & 0xff, &data_027c00d8, (palette + 3) << 12,
                 0x602);

  digit = HwDiv32(value, 100);
  rest = HwMod32(value, 100);
  Gauge_QueueDigitPopIn(x - 1, y + 12, digit, frame, 1);
  digit = HwDiv32(rest, 10);
  rest = HwMod32(rest, 10);
  Gauge_QueueDigitPopIn(x + 11, y + 12, digit, frame - 4, 2);
  Gauge_QueueDigitPopIn(x + 23, y + 12, rest, frame - 8, 3);

  /* Needed to match: barY is computed in both arms. */
  if (value >= 5) {
    barY = (y + 16) & 0xff;
    Sprite_QueueAt(4, (x + 52) & 0x1ff, barY, &data_027c0028, 0x3000, 0x5ba);
  } else {
    barY = (y + 16) & 0xff;
    Sprite_QueueAt(4, (x + 52) & 0x1ff, barY, &data_027c0028, 0x3000,
                   0x5b0 + value * 2);
  }
  for (i = 0; i < BAR_CELLS; i++) {
    if (value >= (i + 1) * BAR_CELL + BAR_CAP) {
      Sprite_QueueAt(4, (x + 60 + i * 8) & 0x1ff, barY, &data_027c0028, 0x3000,
                     0x5cc);
    } else if (value >= i * BAR_CELL + BAR_CAP) {
      Sprite_QueueAt(4, (x + 60 + i * 8) & 0x1ff, barY, &data_027c0028, 0x3000,
                     0x5bc + (value - (i * BAR_CELL + BAR_CAP)) * 2);
    } else {
      Sprite_QueueAt(4, (x + 60 + i * 8) & 0x1ff, barY, &data_027c0028, 0x3000,
                     0x5bc);
    }
  }
  if (value >= 130) {
    Sprite_QueueAt(4, (x + 180) & 0x1ff, barY, &data_027c0028, 0x3000, 0x5d8);
  } else if (value >= 126) {
    Sprite_QueueAt(4, (x + 180) & 0x1ff, barY, &data_027c0028, 0x3000,
                   0x5ce + (value - 126) * 2);
  } else {
    Sprite_QueueAt(4, (x + 180) & 0x1ff, barY, &data_027c0028, 0x3000, 0x5ce);
  }
}
