#include "types.h"
#include "game/move_path.h"

extern MovePath* gMovePath;

/* Direction (MOVE_DIR_*) from tile i of the move path to tile i + 1, or
 * MOVE_DIR_NONE at the last tile or when the tiles are the same. */
int MovePath_DirToNext(int i) {
  if (i == gMovePath->lastTile) {
    return MOVE_DIR_NONE;
  }
  if (gMovePath->x[i] < gMovePath->x[i + 1]) {
    return MOVE_DIR_RIGHT;
  }
  if (gMovePath->x[i] > gMovePath->x[i + 1]) {
    return MOVE_DIR_LEFT;
  }
  if (gMovePath->y[i] < gMovePath->y[i + 1]) {
    return MOVE_DIR_DOWN;
  }
  if (gMovePath->y[i] > gMovePath->y[i + 1]) {
    return MOVE_DIR_UP;
  }
  return MOVE_DIR_NONE;
}
