#include "sdk/snd_seq.h"

/* Right shift the hardware applies for each volume shift setting: 0, 1, 2, 4 bits. */
extern u8 gSndVolumeShiftBits[4];

/* Orders two channels by their output volume: 1 when a is quieter, -1 when louder, 0 when equal. */
int SndExChannel_CompareVolume(SndExChannel* a, SndExChannel* b) {
  int volumeA = a->volume & 0xff;
  int volumeB = b->volume & 0xff;

  volumeA <<= 4;
  volumeB <<= 4;
  volumeA >>= gSndVolumeShiftBits[a->volume >> 8];
  volumeB >>= gSndVolumeShiftBits[b->volume >> 8];
  if (volumeA != volumeB) {
    return volumeA < volumeB ? 1 : -1;
  }
  return 0;
}
