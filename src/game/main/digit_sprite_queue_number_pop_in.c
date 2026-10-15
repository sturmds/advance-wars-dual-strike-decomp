#include "types.h"
#include "game/math.h"

void DigitSprite_QueuePopIn(int x, int y, int digit, int matrixIndex,
                            int frame);

/* Queues a number of up to three digits as pop-in digit sprites, 8 pixels
 * apart. Leading zeros are skipped, and the digits are right-aligned: a one
 * digit value draws at x + 16. Each digit gets the next affine matrix, and
 * starts its pop-in 4 frames after the digit before it. */
void DigitSprite_QueueNumberPopIn(int x, int y, int value, int matrixIndex,
                                  int frame) {
  int hundreds;
  int tens;
  int ones;
  int rest;

  if (value >= 100) {
    hundreds = HwDiv32(value, 100);
    rest = HwMod32(value, 100);
    DigitSprite_QueuePopIn(x, y, hundreds, matrixIndex, frame);
    tens = HwDiv32(rest, 10);
    ones = HwMod32(rest, 10);
    DigitSprite_QueuePopIn(x + 8, y, tens, matrixIndex + 1, frame - 4);
    DigitSprite_QueuePopIn(x + 16, y, ones, matrixIndex + 2, frame - 8);
  } else if (value >= 10) {
    tens = HwDiv32(value, 10);
    ones = HwMod32(value, 10);
    DigitSprite_QueuePopIn(x + 8, y, tens, matrixIndex, frame);
    DigitSprite_QueuePopIn(x + 16, y, ones, matrixIndex + 1, frame - 4);
  } else {
    DigitSprite_QueuePopIn(x + 16, y, value, matrixIndex, frame);
  }
}
