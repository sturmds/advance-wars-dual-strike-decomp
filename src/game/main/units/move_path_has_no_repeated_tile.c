#include "types.h"
#include "game/move_path.h"

extern MovePath* gMovePath;

/* TRUE when no tile of the move path appears twice. */
BOOL MovePath_HasNoRepeatedTile(void) {
  s8 i;
  s8 j;

  for (i = gMovePath->lastTile; i > 0; i--) {
    for (j = i - 1; j >= 0; j--) {
      if (gMovePath->x[i] == gMovePath->x[j] &&
          gMovePath->y[i] == gMovePath->y[j]) {
        return FALSE;
      }
    }
  }
  return TRUE;
}
