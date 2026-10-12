#include "sdk/snd_hw.h"

void SndHw_SetupNoise(int channel, int volume, int volumeShift, int timer, int pan)
{
    int offset = channel << 4;

    gSndHwChannelPan[channel] = pan;
    if (gSndHwPanOverride >= 0) {
        pan = gSndHwPanOverride;
    }
    gSndHwChannelVolume[channel] = volume;
    if (gSndHwPanEdgeAttenuation > 0 && ((1 << channel) & AUDIO_CH_PAN_SCALE_MASK)) {
        volume = SndHw_ScaleVolumeForPan(volume, pan);
    }
    AUDIO_CH_CTRL_WORD(offset) = volume | ((pan << 16) | AUDIO_CH_FORMAT_TONE_GEN | (volumeShift << 8));
    AUDIO_CH_RATE_RELOAD(offset) = 0x10000 - timer;
}
