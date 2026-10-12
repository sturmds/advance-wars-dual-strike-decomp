#include "types.h"

/* How unequal the two axis distances are: | |x0 - x1| - |y0 - y1| |. */
int Map_AxisDistanceGap(int x0, int y0, int x1, int y1) {
  int dx = x0 - x1;
  int dy;
  int diff;

  if (dx <= 0) {
    dx = x1 - x0;
  }
  dy = y0 - y1;
  if (dy <= 0) {
    dy = y1 - y0;
  }
  diff = dx - dy;
  if (diff <= 0) {
    diff = dy - dx;
  }
  return diff;
}
