#include "types.h"

/* Adds a screen corner index to base for the point (x, y): 0 for top left,
 * 1 for top right, 2 for bottom right, 3 for bottom left. The point is in
 * a right (bottom) corner when x >= 224 (y >= 160). */
int Screen_AddCornerIndex(int x, int y, int base) {
  if (x >= 224) {
    if (y >= 160) {
      base += 2;
    } else {
      base += 1;
    }
  } else if (y >= 160) {
    base += 3;
  }
  return base;
}
