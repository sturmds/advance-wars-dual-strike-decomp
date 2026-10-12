#include "sdk/snd.h"

void func_arm7_037fe860(int channel, int volume, int shift);

void SndArm7_SetChannelVolume(u32 channelMask, int volume, int shift)
{
    int channel;

    for (channel = 0; channel < 16 && channelMask != 0; channel++, channelMask >>= 1) {
        if (channelMask & 1) {
            func_arm7_037fe860(channel, volume, shift);
        }
    }
}
