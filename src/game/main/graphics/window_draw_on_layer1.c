#include "types.h"
#include "game/Unk_02088d04.h"

/* draws a window frame of width x height tiles at (x, y) into a 32-tile-wide map */
void Window_Draw(int x, int y, int width, int height, u16* map, int palette);

/* Draws a window frame with palette 0 into the layer 1 tile map. */
void Window_DrawOnLayer1(int x, int y, int width, int height) {
  Window_Draw(x, y, width, height, data_027c0280->layer1Map, 0);
}
