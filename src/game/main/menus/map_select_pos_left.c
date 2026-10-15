#include "types.h"
#include "game/map_select.h"
#include "game/math.h"

int MapSelect_GetPageMapCount(void);

/* Like MapSelect_PosRight, one column to the left. */
int MapSelect_PosLeft(int pos) {
  do {
    if (HwMod32(pos, COLS) != 0) {
      pos--;
    } else {
      pos += COLS - 1;
    }
  } while (pos >= MapSelect_GetPageMapCount());
  return pos;
}
