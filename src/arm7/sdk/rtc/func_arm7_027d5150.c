#include "sdk/rtc.h"

/* Converts 8 BCD digits to binary; 0 if any digit is over 9. */
u32 func_arm7_027d5150(u32 bcd) {
  u32 value = 0;
  u32 scale;
  int i;

  for (i = 0; i < (int)(sizeof(bcd) * BITS_PER_BYTE / 4); i++) {
    if (((bcd >> (i * 4)) & 0xf) >= 10) {
      return 0;
    }
  }
  for (i = 0, scale = 1; i < 8; i++, scale *= 10) {
    value += scale * ((bcd >> (i * 4)) & 0xf);
  }
  return value;
}
