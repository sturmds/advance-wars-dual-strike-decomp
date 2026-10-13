#include "types.h"

/* pixels drawn so far; also declared in src/game/main/text/text_draw_string_to_tiles.c */
int gTextDrawWidth;
extern int
    gTextDrawTilePixel; /* pixel position inside the current tile, 0 to 7 */

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
