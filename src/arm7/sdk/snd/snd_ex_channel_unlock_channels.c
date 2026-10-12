#include "sdk/snd.h"

void SndExChannel_UnlockChannels(u32 channelMask, u32 flags)
{
    if (flags & VOICE_RESERVE_SOFT) {
        data_arm7_03808068 &= ~channelMask;
    } else {
        data_arm7_0380806c &= ~channelMask;
    }
}
