#include "types.h"

/* Direction from the first tile to the second, comparing x before y:
 * 0 same tile, 1 up, 2 right, 3 down, 4 left. */
int Tile_GetDirection(int fromX, int fromY, int toX, int toY) {
  if (fromX > toX) {
    return 4;
  }
  if (fromX < toX) {
    return 2;
  }
  if (fromY > toY) {
    return 1;
  }
  if (fromY < toY) {
    return 3;
  }
  return 0;
}
