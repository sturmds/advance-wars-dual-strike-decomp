#include "types.h"

/* Same as Number_DrawPadded, but draws only as many digits as value needs
 * (at least one). */
void Number_Draw(int x, int y, u16* map, u32 value, u16 entryBits,
                 u16 digitSet) {
  u16* cell = map + x + y * 32;

  do {
    u32 tile = 54 + digitSet * 20 + (value % 10) * 2;
    cell[0] = entryBits | tile;
    cell[32] = entryBits | (tile + 1);
    cell--;
    value /= 10;
  } while (value != 0);
}
