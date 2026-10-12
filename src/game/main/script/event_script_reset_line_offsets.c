#include "types.h"

extern u16 data_022978e0[192];
extern u16 data_027c0240;
extern u16 data_027c024c;

/* Zeroes the 192-entry table data_022978e0 (one entry per screen line; the
 * name is a guess) and sets two offsets to -160. */
void EventScript_ResetLineOffsets(void) {
  s16 i;

  for (i = 0; i < 192; i++) {
    data_022978e0[i] = 0;
  }
  data_027c024c = 0xff60;
  data_027c0240 = 0xff60;
}
