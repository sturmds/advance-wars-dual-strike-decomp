#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;

/* Scrolls the BG that layer 0 uses back to the top left. */
void Layer0_ResetScroll(void) {
  data_027c0280->bgScrollX[data_027c0280->layer0Bg] = 0;
  data_027c0280->bgScrollY[data_027c0280->layer0Bg] = 0;
}
