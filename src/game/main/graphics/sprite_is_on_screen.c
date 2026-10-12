#include "types.h"

/* Nonzero when a point is on the 256x192 screen, widened by half of size on
 * every side so a sprite of that size centered there still shows. */
BOOL Sprite_IsOnScreen(int x, int y, int size) {
  int margin = size >> 1;

  if (x < -margin || margin + 256 < x || y < -margin || margin + 192 < y) {
    return 0;
  }
  return 1;
}
