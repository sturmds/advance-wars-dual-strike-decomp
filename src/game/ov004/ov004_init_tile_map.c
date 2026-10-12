#include "types.h"
#include "game/Unk_ov004_022bf158.h"

extern void* data_ov004_022bf154;

void Ov004_InitTileMap(Unk_ov004_022bf158* state, int blocksWide,
                       int blocksHigh) {
  state->blocksWide = blocksWide;
  state->blocksHigh = blocksHigh;
  state->viewX = 0;
  state->viewY = 0;
  state->viewScrollX = 0;
  state->viewScrollY = 0;
  state->tileMap = data_ov004_022bf154;
}
