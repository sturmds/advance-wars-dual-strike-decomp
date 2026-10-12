/* Needed to match: the original keeps the pan load ahead of the call. */
#pragma opt_dead_assignments off
#include "sdk/snd_hw.h"

void SndHw_SetChannelVolume(int channel, int volume, int volumeShift)
{
    gSndHwChannelVolume[channel] = volume;
    if (gSndHwPanEdgeAttenuation > 0 && ((1 << channel) & AUDIO_CH_PAN_SCALE_MASK)) {
        int pan = AUDIO_CH_BALANCE_BYTE(channel << 4);

        volume = SndHw_ScaleVolumeForPan(volume, pan);
    }
    AUDIO_CH_LOUDNESS_DIVIDER(channel << 4) = volume | (volumeShift << 8);
}
