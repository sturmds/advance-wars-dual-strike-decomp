#include "types.h"
#include "game/Unk_0229c734.h"

s32 HwDiv32(s32 numer, s32 denom);
s32 HwMod32(s32 numerator, s32 denominator);
void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);

extern u8 data_027c0040[];

#define SCORES data_0229c734

#define QUEUE_DIGIT(x, y, digit, palette)                   \
  Sprite_QueueAt(6, (x) & 0x1ff, (y) & 0xff, data_027c0040, \
                 (SCORES->paletteSlot + (palette)) << 12,   \
                 SCORES->partTile[3] + 32 + (digit) * 4)

/* Queues a number as result point digit sprites, 12 pixels apart, in
 * palette paletteSlot + palette. Values above 9999 show as 9999. The digits
 * are right-aligned with the ones at x + 24, so a fourth digit starts at
 * x - 12. Leading zeros are skipped; once a digit is drawn, every lower
 * digit is drawn too, which the gotos do. A negative value draws nothing. */
void ResultPoint_QueueNumber(int x, int y, int value, int palette) {
  int digit;

  if (value > 9999) {
    value = 9999;
  }
  if (value >= 1000) {
    digit = HwDiv32(value, 1000);
    QUEUE_DIGIT(x - 12, y, digit, palette);
    value = HwMod32(value, 1000);
    goto hundreds;
  }
  if (value >= 100) {
  hundreds:
    digit = HwDiv32(value, 100);
    QUEUE_DIGIT(x, y, digit, palette);
    value = HwMod32(value, 100);
    goto tens;
  }
  if (value >= 10) {
  tens:
    digit = HwDiv32(value, 10);
    QUEUE_DIGIT(x + 12, y, digit, palette);
    value = HwMod32(value, 10);
    goto ones;
  }
  if (value < 0) {
    return;
  }
ones:
  QUEUE_DIGIT(x + 24, y, value, palette);
}
