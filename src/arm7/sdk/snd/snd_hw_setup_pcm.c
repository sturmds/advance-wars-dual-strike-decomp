#include "sdk/snd_hw.h"

void SndHw_SetupPcm(int channel, u32 source, int format, int repeatMode,
                    int loopStart, int length, int volume, int volumeShift,
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
  AUDIO_CH_CTRL_WORD(offset) = volume | ((format << 29) | (repeatMode << 27) |
                                         (pan << 16) | (volumeShift << 8));
  AUDIO_CH_RATE_RELOAD(offset) = 0x10000 - timer;
  AUDIO_CH_REPEAT_POINT(offset) = loopStart;
  AUDIO_CH_DATA_SPAN(offset) = length;
  AUDIO_CH_SAMPLE_ADDR(offset) = source;
}
