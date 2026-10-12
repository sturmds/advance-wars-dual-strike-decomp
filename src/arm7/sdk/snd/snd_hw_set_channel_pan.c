#include "sdk/snd_hw.h"

void SndHw_SetChannelPan(int channel, int pan)
{
    int offset;

    gSndHwChannelPan[channel] = pan;
    if (gSndHwPanOverride >= 0) {
        pan = gSndHwPanOverride;
    }
    offset = channel << 4;
    AUDIO_CH_BALANCE_BYTE(offset) = pan;
    if (gSndHwPanEdgeAttenuation > 0 && ((1 << channel) & AUDIO_CH_PAN_SCALE_MASK)) {
        AUDIO_CH_LOUDNESS_BYTE(offset) = SndHw_ScaleVolumeForPan(gSndHwChannelVolume[channel], pan);
    }
}
