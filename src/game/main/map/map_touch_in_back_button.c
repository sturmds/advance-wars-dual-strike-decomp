#include "types.h"

/* TRUE when the touch (x, y) is on the 32x16 button at x 112, y 0. */
BOOL MapTouch_InBackButton(int x, int y) {
  if (x >= 112 && x <= 144 && y >= 0 && y <= 16) {
    return TRUE;
  }
  return FALSE;
}
