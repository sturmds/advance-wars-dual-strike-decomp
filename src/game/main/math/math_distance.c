#include "types.h"
#include "game/math.h"

/* Distance between (x1, y1) and (x2, y2), rounded down. */
int Math_Distance(int x1, int y1, int x2, int y2) {
  return Math_Sqrt32((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}
