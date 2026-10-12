#include "types.h"
#include "sdk/msl.h"

void __timesdec(Decimal* result, const Decimal* x, const Decimal* y) {
  u32 accumulator = 0;
  u8 product[2 * DECIMAL_SIG_DIGITS];
  int i = x->length + y->length - 1;
  u8* out = product + i + 1;
  u8* end = out;

  result->sign = 0;
  /* Column sums from the last digit up; each column adds the pairs x[j] * y[k] with j + k = i - 1. */
  for (; i > 0; i--) {
    int k = y->length - 1;
    int j = i - k - 1;
    int count;
    int xLeft;
    const u8* xp;
    const u8* yp;

    if (j < 0) {
      j = 0;
      k = i - 1;
    }
    xp = x->digits + j;
    yp = y->digits + k;
    count = k + 1;
    xLeft = x->length - j;
    if (count > xLeft) {
      count = xLeft;
    }
    for (; count > 0; count--, xp++, yp--) {
      accumulator += (u32)*xp * *yp;
    }
    *--out = accumulator % 10;
    accumulator /= 10;
  }
  result->exp = x->exp + y->exp;
  if (accumulator > 0) {
    *--out = accumulator;
    result->exp++;
  }
  for (i = 0; i < DECIMAL_SIG_DIGITS && out < end; i++, out++) {
    result->digits[i] = *out;
  }
  result->length = i;
  /* Round half to even on the digits that did not fit. */
  if (out < end && *out >= 5) {
    if (*out == 5) {
      u8* p = out + 1;

      for (; p < end; p++) {
        if (*p != 0) {
          goto round_up;
        }
      }
      if ((*(out - 1) & 1) == 0) {
        return;
      }
    }
  round_up:
    __dorounddecup(result, result->length);
  }
}
