#include "sdk/snd.h"

void SND_UnlockChannel(u32 channelMask, u32 flags)
{
    Sound_PostCommandArgs(25, channelMask, flags, 0, 0);
}
