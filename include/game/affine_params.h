#ifndef GAME_AFFINE_PARAMS_H
#define GAME_AFFINE_PARAMS_H

#include "types.h"
#include "game/Unk_02088d04.h"

/* Input of BgAffine_FromParams, which turns it into the matrix of a BgAffine. */
typedef struct AffineParams {
  s32 srcX; /* 256: BG pixel drawn at (centerX, centerY) */
  s32 srcY;
  s16 centerX;
  s16 centerY;
  s16 scaleX; /* 256: 1.0 */
  s16 scaleY;
  u16 angle;
} AffineParams;

void BgAffine_FromParams(AffineParams* params, BgAffine* dest);

#endif
