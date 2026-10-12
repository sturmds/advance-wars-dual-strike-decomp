#include "sdk/snd.h"

void func_arm7_037fe7cc(int channel, int pan);

void SndArm7_SetChannelPan(u32 channelMask, int pan)
{
    int channel;

    for (channel = 0; channel < 16 && channelMask != 0; channel++, channelMask >>= 1) {
        if (channelMask & 1) {
            func_arm7_037fe7cc(channel, pan);
        }
    }
}
