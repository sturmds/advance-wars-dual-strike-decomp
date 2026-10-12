#include "types.h"

/* Moves OAM entries below the screen (y = 192), 16 at a time, so they are
 * not drawn. Entries are 8 bytes; count is rounded down to a multiple of 16. */
void Oam_HideEntries(void* oam, int count) {
  u32* entries = oam;

  for (count = (count >> 4) - 1; count >= 0; count--) {
    entries[0] = 192;
    entries[2] = 192;
    entries[4] = 192;
    entries[6] = 192;
    entries[8] = 192;
    entries[10] = 192;
    entries[12] = 192;
    entries[14] = 192;
    entries[16] = 192;
    entries[18] = 192;
    entries[20] = 192;
    entries[22] = 192;
    entries[24] = 192;
    entries[26] = 192;
    entries[28] = 192;
    entries[30] = 192;
    entries += 32;
  }
}
