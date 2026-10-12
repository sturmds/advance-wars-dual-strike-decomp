#include "types.h"
#include "game/map_select.h"

int HwDiv32(int a, int b);
int HwMod32(int a, int b);
int MapSelect_GetPageMapCount(void);

/* Position on the page one row down from pos; the last row wraps to the first
 * row of the same column. Repeats while the position holds no map. */
int MapSelect_PosDown(int pos) {
  int row; /* needed to match: as in MapSelect_PosRight */

  do {
    row = HwDiv32(pos, COLS);
    if (row < ROWS - 1) {
      pos += COLS;
    } else {
      pos = HwMod32(pos, COLS);
    }
  } while (pos >= MapSelect_GetPageMapCount());
  return pos;
}
