#include "types.h"
#include "game/map_select.h"
#include "game/math.h"

int MapSelect_GetPageMapCount(void);

/* Position on the page one column to the right of pos; the last column wraps
 * to the first of the same row. Repeats while the position holds no map. */
int MapSelect_PosRight(int pos) {
  int col; /* needed to match: keeps the compare from reusing COLS from before the call */

  do {
    col = HwMod32(pos, COLS);
    if (col < COLS - 1) {
      pos++;
    } else {
      pos -= COLS - 1;
    }
  } while (pos >= MapSelect_GetPageMapCount());
  return pos;
}
