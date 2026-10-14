#include "types.h"
#include "game/Unk_02088d04.h"

extern u16 data_022978e0[];
extern s16 data_027c0240;
extern s16 data_027c024c;

/* Zeroes the 192-entry table data_022978e0 (one entry per screen line; the
 * name is a guess) and sets two offsets to -160. */
void EventScript_ResetLineOffsets(void) {
  s16 i;

  for (i = 0; i < SCREEN_HEIGHT; i++) {
    data_022978e0[i] = 0;
  }
  /* needed to match: the original stores the halfword as unsigned */
  *(u16*)&data_027c024c = 0xff60;
  *(u16*)&data_027c0240 = 0xff60;
}
