#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;

/* Scrolls layer 0 to (x, y) by setting the scroll of the BG it uses. */
void Screen_SetLayer0Scroll(u16 x, u16 y) {
  data_027c0280->bgScrollX[data_027c0280->layer0Bg] = x;
  data_027c0280->bgScrollY[data_027c0280->layer0Bg] = y;
}
