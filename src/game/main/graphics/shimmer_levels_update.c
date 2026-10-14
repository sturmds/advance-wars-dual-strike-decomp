#include "types.h"
#include "game/shimmer_wave.h"

extern u16 data_022a7c64[];

/* Sets the brightness (0 to 31) of the four quads that Gx_SendQuad draws.
 * Each quad follows its own wave: full brightness with one dip that moves
 * from quad to quad. The waves advance one step every two frames and repeat
 * every 128 frames. */
void ShimmerLevels_Update(int frame) {
  ShimmerWave wave0 = data_0216a25c;
  ShimmerWave wave1 = data_0216a2dc;
  ShimmerWave wave2 = data_0216a35c;
  ShimmerWave wave3 = data_0216a3dc;
  int step = (frame & 127) >> 1;

  data_022a7c64[0] = wave0.level[step];
  data_022a7c64[1] = wave1.level[step];
  data_022a7c64[2] = wave2.level[step];
  data_022a7c64[3] = wave3.level[step];
}
