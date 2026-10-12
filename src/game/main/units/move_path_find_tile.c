#include "types.h"
#include "game/move_path.h"

extern MovePath* gMovePath;

/* Index of the move path tile at (x, y), or -1 when the path does not pass it. */
int MovePath_FindTile(s8 x, s8 y) {
  s8 i;

  for (i = 0; i <= gMovePath->lastTile; i++) {
    if (x == gMovePath->x[i] && y == gMovePath->y[i]) {
      return i;
    }
  }
  return -1;
}
