#include "sdk/snd.h"

void SND_LockChannel(u32 channelMask, u32 flags)
{
    Sound_PostCommandArgs(24, channelMask, flags, 0, 0);
}
