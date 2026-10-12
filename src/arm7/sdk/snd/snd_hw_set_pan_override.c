#include "sdk/snd_hw.h"

void SndHw_SetPanOverride(int pan) {
  int channel;

  gSndHwPanOverride = pan;
  if (pan >= 0) {
    for (channel = 0; channel < AUDIO_CH_TOTAL; channel++) {
      AUDIO_CH_REG_BLOCKS[channel].pan = pan;
    }
  } else {
    for (channel = 0; channel < AUDIO_CH_TOTAL; channel++) {
      AUDIO_CH_REG_BLOCKS[channel].pan = gSndHwChannelPan[channel];
    }
  }
}
