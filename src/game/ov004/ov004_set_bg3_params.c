#include "types.h"
#include "game/Unk_ov004_022bf158.h"

void Ov004_SetBg3Params(Unk_ov004_022bf158* state, u32 srcX, u32 srcY,
                        u16 centerX, u16 centerY, u16 scaleX, u16 scaleY,
                        u16 angle) {
  AffineParams* params = &state->bg3Params;

  params->srcX = srcX;
  params->srcY = srcY;
  params->centerX = centerX;
  params->centerY = centerY;
  params->scaleX = scaleX;
  params->scaleY = scaleY;
  params->angle = angle;
}
