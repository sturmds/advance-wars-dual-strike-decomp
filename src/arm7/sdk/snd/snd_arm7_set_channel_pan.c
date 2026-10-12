#include "sdk/snd.h"

void SndHw_SetChannelPan(int channel, int pan);

void SndArm7_SetChannelPan(u32 channelMask, int pan)
{
    int channel;

    for (channel = 0; channel < 16 && channelMask != 0; channel++, channelMask >>= 1) {
        if (channelMask & 1) {
            SndHw_SetChannelPan(channel, pan);
        }
    }
}
