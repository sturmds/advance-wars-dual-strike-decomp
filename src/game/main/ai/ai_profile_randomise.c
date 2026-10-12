#include "types.h"

s32 HwDiv32(s32 num, s32 den);

/* Returns base + spread * roll / 100 - spread / 2, kept within 1-255. */
int AiProfile_Randomise(u8 base, u8 spread, int roll) {
  int value = base + (HwDiv32(spread * roll, 100) - (spread >> 1));

  if (value < 1) {
    value = 1;
  }
  if (value > 255) {
    value = 255;
  }
  return value;
}
