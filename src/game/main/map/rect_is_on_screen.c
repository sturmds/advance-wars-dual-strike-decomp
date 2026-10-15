#include "types.h"
#include "game/Unk_02080440.h"

/* TRUE when a width by height rectangle with its top left corner at map
 * pixel (x, y) overlaps the 256x192 screen at the current view scroll. */
BOOL Rect_IsOnScreen(int x, int y, int width, int height) {
  Unk_02080440_1c4* view = MAP_STATE->unk_1c4;
  int screenY = y - view->scrollY;
  int screenX;

  if (screenY > -height && screenY < 192) {
    screenX = x - view->scrollX;
    if (screenX > -width && screenX < 256) {
      return TRUE;
    }
  }
  return FALSE;
}
