#include "sdk/snd.h"
#include "sdk/snd_hw.h"

/* Hardware channel numbers in the order the allocator tries them. */
extern u8 gSndExChannelAllocOrder[AUDIO_CH_TOTAL];

/*
 * Picks a channel from channelMask for a new sound of the given priority: a free channel, else the one of
 * lowest priority, the quietest among equals. Returns NULL when every candidate outranks the request.
 */
SndExChannel *SndExChannel_Alloc(u32 channelMask, int priority, BOOL strong, SndExChannelCallback callback, void *callbackArg)
{
    SndExChannel *best;
    int i;

    channelMask &= ~data_arm7_0380806c;
    if (!strong) {
        channelMask &= ~data_arm7_03808068;
    }
    best = NULL;
    for (i = 0; i < AUDIO_CH_TOTAL; i++) {
        int id = gSndExChannelAllocOrder[i];

        if (channelMask & (1 << id)) {
            SndExChannel *channel = &data_arm7_03808090.channels[id];

            if (best == NULL) {
                best = channel;
            } else if (channel->priority <= best->priority) {
                if (channel->priority != best->priority || SndExChannel_CompareVolume(best, channel) < 0) {
                    best = channel;
                }
            }
        }
    }
    if (best == NULL) {
        return NULL;
    }
    if (priority < best->priority) {
        return NULL;
    }
    if (best->callback != NULL) {
        best->callback(best, VOICE_STOLEN, best->callbackArg);
    }
    best->sync = VOICE_DIRTY_STOP;
    best->active = FALSE;
    SndExChannel_Init(best, callback, callbackArg, priority);
    return best;
}
