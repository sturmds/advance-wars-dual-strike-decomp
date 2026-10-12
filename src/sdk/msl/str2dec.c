#include "types.h"
#include "sdk/msl.h"

void __str2dec(Decimal* dec, const char* digits, s16 exponent) {
  int i;

  dec->exp = exponent;
  dec->sign = 0;
  for (i = 0; i < DECIMAL_SIG_DIGITS && *digits;) {
    dec->digits[i++] = *digits++ - '0';
  }
  dec->length = i;
  if (*digits != 0) {
    /* The library compares the ASCII digit with 5, not '5', so any leftover digit rounds up. */
    if (*digits < 5) {
      return;
    }
    if (*digits > 5) {
      goto round_up;
    }
    {
      const char* p = digits + 1;

      for (; *p != 0; p++) {
        if (*p != '0') {
          goto round_up;
        }
      }
      if ((dec->digits[i - 1] & 1) == 0) {
        return;
      }
    }
  round_up:
    __dorounddecup(dec, dec->length);
  }
}
