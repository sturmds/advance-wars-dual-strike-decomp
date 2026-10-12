#include "types.h"
#include "game/affine_params.h"

s32 HwDiv32(s32 numerator, s32 denominator);

extern const s16 gSineTable
    []; /* sine table: 256 steps per turn, 12 fraction bits; +64 gives cosine */

/* Builds the rotation and scale matrix of a BG2 or BG3 affine layer. It maps
 * the BG pixel (srcX, srcY) to the screen point (centerX, centerY). */
void BgAffine_FromParams(AffineParams* params, BgAffine* dest) {
  s32 srcX;
  s32 srcY;
  s32 dx;
  s32 dy;
  s32 sin;
  s32 cos;
  u8 angle;
  angle = params->angle;
  cos = gSineTable[angle + 64] << 8;
  srcX = params->srcX;
  srcY = params->srcY;
  dx = srcX - params->centerX;
  dy = srcY - params->centerY;

  dest->pa = (u32)HwDiv32(cos, params->scaleX) >> 4;
  sin = gSineTable[angle] << 8;
  dest->pb = (u32)HwDiv32(sin, params->scaleX) >> 4;
  dest->pc = (u32)-HwDiv32(sin, params->scaleY) >> 4;
  dest->pd = (u32)HwDiv32(cos, params->scaleY) >> 4;
  dest->x = dest->pa * (dx - srcX) + dest->pb * (dy - srcY) + (srcX << 8);
  dest->y = dest->pc * (dx - srcX) + dest->pd * (dy - srcY) + (srcY << 8);
}
