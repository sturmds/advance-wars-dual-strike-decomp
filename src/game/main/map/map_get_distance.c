#include "types.h"

/* Manhattan distance in tiles. */
int Map_GetDistance(int x1, int y1, int x2, int y2) {
  int dist = 0;

  if (x1 - x2 > 0) {
    dist += x1 - x2;
  } else {
    dist += x2 - x1;
  }
  if (y1 - y2 > 0) {
    dist += y1 - y2;
  } else {
    dist += y2 - y1;
  }
  return dist;
}
