#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;

/* Draw order, front to back: layer 1, layer 0, layer 2, layer 3. */
void Screen_SetLayer1Front(void) {
  data_027c0280->bgCnt[data_027c0280->layer0Bg].priority = 1;
  data_027c0280->bgCnt[data_027c0280->layer1Bg].priority = 0;
  data_027c0280->bgCnt[data_027c0280->layer2Bg].priority = 2;
  data_027c0280->bgCnt[data_027c0280->layer3Bg].priority = 3;
}
