#include "types.h"

/* TRUE when a sprite at x with this width is at least partly on the
 * 256-pixel-wide screen. */
BOOL Sprite_IsOnScreenX(int x, int width) {
  if (x < -width || x >= 256) {
    return FALSE;
  }
  return TRUE;
}
