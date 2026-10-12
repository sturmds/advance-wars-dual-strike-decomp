#include "types.h"

extern u8 data_0216e2b0;

/* Sets the hidden/shown nibble of both screens in data_0216e2b0 byte 0
 * (low nibble: main screen, high nibble: sub screen). */
void Screen_SetBothNibbles(int value) {
  int nibble = value & 15;

  data_0216e2b0 = (data_0216e2b0 & ~15) | nibble;
  data_0216e2b0 = (data_0216e2b0 & ~0xf0) | (nibble << 4);
}
