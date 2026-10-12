#include "types.h"
#include "game/menu_button.h"

/* Moves the menu cursor corners to a rectangle at once; MenuCursor_Update
 * then eases them from there. */
void MenuCursor_SetRect(s16 left, s16 top, s16 right, s16 bottom) {
  gMenuCursor->corners[0][0] = left;
  gMenuCursor->corners[0][1] = top;
  gMenuCursor->corners[1][0] = right;
  gMenuCursor->corners[1][1] = top;
  gMenuCursor->corners[2][0] = left;
  gMenuCursor->corners[2][1] = bottom;
  gMenuCursor->corners[3][0] = right;
  gMenuCursor->corners[3][1] = bottom;
}
