#include "types.h"
#include "game/map_select.h"

int HwDiv32(int a, int b);
int MapSelect_GetPageMapCount(void);

/* Like MapSelect_PosDown, one row up; the first row wraps to the last row of
 * the same column. */
int MapSelect_PosUp(int pos) {
  do {
    if (HwDiv32(pos, COLS) != 0) {
      pos -= COLS;
    } else {
      pos += (ROWS - 1) * COLS;
    }
  } while (pos >= MapSelect_GetPageMapCount());
  return pos;
}
