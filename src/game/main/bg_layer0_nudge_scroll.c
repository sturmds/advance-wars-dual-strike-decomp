#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;

/* Moves the scroll of layer 0's BG: up 12, right 10. */
void BgLayer0_NudgeScroll(void) {
  data_027c0280->bgScrollY[data_027c0280->layer0Bg] -= 12;
  data_027c0280->bgScrollX[data_027c0280->layer0Bg] += 10;
}
