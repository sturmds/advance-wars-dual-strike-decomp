#include "types.h"

extern u32 data_022a804c;                  /* sound channels locked so far, one bit each */

void func_0210fce8(u32 channelMask, int flags);

/* Takes the channels in channelMask away from the sound processor. Returns
 * FALSE, locking nothing, if any of them is locked already. */
BOOL Sound_LockChannels(u32 channelMask)
{
    if (channelMask == 0) {
        return TRUE;
    }
    if (channelMask & data_022a804c) {
        return FALSE;
    }
    func_0210fce8(channelMask, 0);
    data_022a804c |= channelMask;
    return TRUE;
}
