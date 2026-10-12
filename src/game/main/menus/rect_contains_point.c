#include "types.h"
#include "game/rect.h"

/* Edges count as inside: x runs from rect->x to rect->x + rect->width. */
BOOL Rect_ContainsPoint(int x, int y, const Rect* rect) {
  if (rect->x <= x && rect->y <= y && rect->x + rect->width >= x &&
      rect->y + rect->height >= y) {
    return TRUE;
  }
  return FALSE;
}
