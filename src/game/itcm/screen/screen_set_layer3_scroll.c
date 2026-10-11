#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280 *data_027c0280;

/* Scrolls layer 3 to (x, y) by setting the scroll of the BG it uses. */
void Screen_SetLayer3Scroll(u16 x, u16 y)
{
    data_027c0280->bgScrollX[data_027c0280->layer3Bg] = x;
    data_027c0280->bgScrollY[data_027c0280->layer3Bg] = y;
}
