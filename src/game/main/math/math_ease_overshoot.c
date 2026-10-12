#include "types.h"

extern s16 data_0214d4c4[]; /* a sine and cosine pair per 16 angle units */

s32 HwDiv32(s32 numerator, s32 denominator);

#define FX_MUL(a, b) ((fx32)(((long long)(a) * (b) + 0x800) >> 12))

/* Linear ease from `from` to `to` over `duration` frames, plus a sine of the
 * progress scaled by amount / 314: the value overshoots on the way. */
int Math_EaseOvershoot(int amount, int from, int to, int elapsed,
                       int duration) {
  int progress = HwDiv32(elapsed << 12, duration);
  int height = HwDiv32(amount << 12, 314);
  int sum;

  sum = progress;
  sum += FX_MUL(height, data_0214d4c4[((progress * 8) >> 4) * 2]);
  return from + ((sum * (to - from)) >> 12);
}
