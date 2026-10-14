#ifndef GAME_SHIMMER_WAVE_H
#define GAME_SHIMMER_WAVE_H

#include "types.h"

#define SHIMMER_WAVE_STEPS 128

/* The brightness (0 to 31) of one of Gx_SendQuad's four quads at each step
 * (ShimmerLevels_Update). A struct so that the wave is copied whole, as the
 * code does. */
typedef struct ShimmerWave {
  u8 level[SHIMMER_WAVE_STEPS];
} ShimmerWave;

extern ShimmerWave data_0216a25c;
extern ShimmerWave data_0216a2dc;
extern ShimmerWave data_0216a35c;
extern ShimmerWave data_0216a3dc;

#endif
