#include "types.h"
#include "game/text_print.h"

int gTextDrawWidth;

/* Moves the drawing position of the string being drawn right by pixels.
 * Returns TRUE when it crossed into the next tile. */
BOOL Text_AdvanceDrawPos(int pixels) {
  gTextDrawTilePixel += pixels;
  gTextDrawWidth += pixels;
  if (gTextDrawTilePixel >= 8) {
    gTextDrawTilePixel -= 8;
    return TRUE;
  }
  return FALSE;
}
