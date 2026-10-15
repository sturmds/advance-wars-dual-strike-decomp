#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Window_DrawFrameStriped(int x, int y, int width, int height, u16* map,
                             int arg);

/* Draws a framed box of width x height tiles at (x, y) on the layer 1 map,
 * then runs the state's afterBoxDraw hook if it has one. */
void MenuBox_Draw(int x, int y, int width, int height) {
  Window_DrawFrameStriped(x, y, width, height, data_027c0280->layer1Map, 0);
  if (data_027c027c->unk_50[data_027c0284]->afterBoxDraw != NULL) {
    data_027c027c->unk_50[data_027c0284]->afterBoxDraw();
  }
}
