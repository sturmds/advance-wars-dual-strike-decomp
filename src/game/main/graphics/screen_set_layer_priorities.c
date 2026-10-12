#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;

/* Gives the four layers the BG priorities 0 to 3, layer 0 in front. */
void Screen_SetLayerPriorities(void) {
  data_027c0280->bgCnt[data_027c0280->layer0Bg].priority = 0;
  data_027c0280->bgCnt[data_027c0280->layer1Bg].priority = 1;
  data_027c0280->bgCnt[data_027c0280->layer2Bg].priority = 2;
  data_027c0280->bgCnt[data_027c0280->layer3Bg].priority = 3;
}
