#include "types.h"
#include "game/Unk_ov004_022bf158.h"

void Ov004_SetBg3Scale(Unk_ov004_022bf158* state, u16 x, u16 y) {
  AffineParams* params = &state->bg3Params;

  params->scaleX = x;
  params->scaleY = y;
}
