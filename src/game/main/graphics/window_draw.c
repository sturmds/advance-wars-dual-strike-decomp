#include "types.h"

void Window_DrawFrame(int x, int y, int width, int height, u16* map,
                      int palette);

/* Draws a window frame of width x height tiles at tile (x, y) of map; the
 * work is done by Window_DrawFrame. */
void Window_Draw(int x, int y, int width, int height, u16* map, int palette) {
  Window_DrawFrame(x, y, width, height, map, palette);
}
