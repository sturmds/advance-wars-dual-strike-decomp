#include "sdk/snd_hw.h"

void SndHw_SetupPsg(int channel, int duty, int volume, int volumeShift,
                    int timer, int pan) {
  int offset = channel << 4;

  gSndHwChannelPan[channel] = pan;
  if (gSndHwPanOverride >= 0) {
    pan = gSndHwPanOverride;
  }
  gSndHwChannelVolume[channel] = volume;
  if (gSndHwPanEdgeAttenuation > 0 &&
      ((1 << channel) & AUDIO_CH_PAN_SCALE_MASK)) {
    volume = SndHw_ScaleVolumeForPan(volume, pan);
  }
  AUDIO_CH_CTRL_WORD(offset) =
      volume | ((duty << 24) | AUDIO_CH_FORMAT_TONE_GEN | (pan << 16) |
                (volumeShift << 8));
  AUDIO_CH_RATE_RELOAD(offset) = 0x10000 - timer;
}
