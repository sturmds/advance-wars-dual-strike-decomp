#include "types.h"
#include "game/move_path.h"

extern MovePath* gMovePath;

/* Direction (MOVE_DIR_*) from tile i of the move path back to tile i - 1,
 * or MOVE_DIR_NONE at tile 0 or when the tiles are the same. */
int MovePath_DirToPrev(int i) {
  if (i == 0) {
    return MOVE_DIR_NONE;
  }
  if (gMovePath->x[i - 1] < gMovePath->x[i]) {
    return MOVE_DIR_LEFT;
  }
  if (gMovePath->x[i - 1] > gMovePath->x[i]) {
    return MOVE_DIR_RIGHT;
  }
  if (gMovePath->y[i - 1] < gMovePath->y[i]) {
    return MOVE_DIR_UP;
  }
  if (gMovePath->y[i - 1] > gMovePath->y[i]) {
    return MOVE_DIR_DOWN;
  }
  return MOVE_DIR_NONE;
}
