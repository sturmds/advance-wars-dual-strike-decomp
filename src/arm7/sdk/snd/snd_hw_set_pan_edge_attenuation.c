/* Needed to match: the original keeps the pan load ahead of the volume load. */
#pragma opt_dead_assignments off
#include "sdk/snd_hw.h"

void SndHw_SetPanEdgeAttenuation(int attenuation)
{
    int channel;

    gSndHwPanEdgeAttenuation = attenuation;
    for (channel = 0; channel < AUDIO_CH_TOTAL; channel++) {
        if ((1 << channel) & AUDIO_CH_PAN_SCALE_MASK) {
            int offset = channel << 4;
            int pan = AUDIO_CH_BALANCE_BYTE(offset);

            AUDIO_CH_LOUDNESS_BYTE(offset) = SndHw_ScaleVolumeForPan(gSndHwChannelVolume[channel], pan);
        }
    }
}
