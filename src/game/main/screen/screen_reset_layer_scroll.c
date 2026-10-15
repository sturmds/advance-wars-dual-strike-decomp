#include "types.h"
#include "game/Unk_02088d04.h"

/* Sets the scroll position of the BG each of the four layers uses to 0, 0. */
void Screen_ResetLayerScroll(void) {
  data_027c0280->bgScrollX[GFX_LAYER(data_027c0280, 0)->bg] = 0;
  data_027c0280->bgScrollY[GFX_LAYER(data_027c0280, 0)->bg] = 0;
  data_027c0280->bgScrollX[GFX_LAYER(data_027c0280, 1)->bg] = 0;
  data_027c0280->bgScrollY[GFX_LAYER(data_027c0280, 1)->bg] = 0;
  data_027c0280->bgScrollX[GFX_LAYER(data_027c0280, 2)->bg] = 0;
  data_027c0280->bgScrollY[GFX_LAYER(data_027c0280, 2)->bg] = 0;
  data_027c0280->bgScrollX[GFX_LAYER(data_027c0280, 3)->bg] = 0;
  data_027c0280->bgScrollY[GFX_LAYER(data_027c0280, 3)->bg] = 0;
}
