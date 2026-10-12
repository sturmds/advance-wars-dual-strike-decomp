#include "types.h"

s32 HwDiv32(s32 numer, s32 denom);
s32 HwMod32(s32 numerator, s32 denominator);
void RankSprite_QueueHundredPopIn(int x, int y, int matrixIndex, int frame);
void RankSprite_QueueDigitPopIn(int x, int y, int digit, int matrixIndex,
                                int frame);

/* Queues a CO rank (1-100) with the pop-in: the rank-100 sprite, or one or
 * two digits. */
void RankSprite_QueueNumber(int x, int y, int value, int matrixIndex,
                            int frame) {
  int tens;

  if (value == 100) {
    RankSprite_QueueHundredPopIn(x, y, matrixIndex, frame);
    return;
  }
  if (value >= 10) {
    tens = HwDiv32(value, 10);
    value = HwMod32(value, 10);
    RankSprite_QueueDigitPopIn(x, y, tens, matrixIndex, frame);
  }
  RankSprite_QueueDigitPopIn(x + 8, y, value, matrixIndex + 1, frame);
}
