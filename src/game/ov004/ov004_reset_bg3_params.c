#include "types.h"
#include "game/Unk_ov004_022bf158.h"

void Ov004_ResetBg3Params(Unk_ov004_022bf158* state) {
  AffineParams* params = &state->bg3Params;

  state->bg3Params.srcX = 0;
  params->srcY = 0;
  params->centerX = 0;
  params->centerY = 0;
  params->scaleX = 256;
  params->scaleY = 256;
  params->angle = 0;
}
