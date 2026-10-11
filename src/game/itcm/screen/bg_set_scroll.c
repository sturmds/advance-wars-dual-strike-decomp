#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280 *data_027c0280;

/* Scrolls layer 0 to 3 to (x, y). Layers marked GFX_LAYER_NO_COPY are
 * skipped. */
void Bg_SetScroll(int layer, u16 x, u16 y)
{
    if (GFX_LAYERS(data_027c0280)[layer].mode == GFX_LAYER_NO_COPY) {
        return;
    }
    data_027c0280->bgScrollX[GFX_LAYERS(data_027c0280)[layer].bg] = x;
    data_027c0280->bgScrollY[GFX_LAYERS(data_027c0280)[layer].bg] = y;
}
