#include "types.h"
#include "game/Unk_ov004_022bf158.h"

void Ov004_SetBg3Center(Unk_ov004_022bf158* state, u16 x, u16 y) {
  AffineParams* params = &state->bg3Params;

  params->centerX = x;
  params->centerY = y;
}
