#include "types.h"

extern u32 gLockedSoundChannels;                  /* sound channels locked so far, one bit each */

void func_0210fcbc(u32 channelMask, int flags);

/* Gives the channels in channelMask back to the sound processor. */
void Sound_UnlockChannels(u32 channelMask)
{
    if (channelMask == 0) {
        return;
    }
    func_0210fcbc(channelMask, 0);
    gLockedSoundChannels &= ~channelMask;
}
