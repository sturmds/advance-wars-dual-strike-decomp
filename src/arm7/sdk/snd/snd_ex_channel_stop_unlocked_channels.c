#include "sdk/snd.h"
#include "sdk/snd_hw.h"

void func_arm7_037ff68c(SndExChannel *channel);

/* Stops the channels in channelMask, skipping locked ones; weakly locked channels are stopped too. */
void SndExChannel_StopUnlockedChannels(u32 channelMask, u32 flags)
{
    SndExChannel *channel;
    int i;

    for (i = 0; i < AUDIO_CH_TOTAL && channelMask != 0; i++, channelMask >>= 1) {
        if (!(channelMask & 1)) {
            continue;
        }
        channel = &data_arm7_03808090.channels[i];
        if (data_arm7_0380806c & (1 << i)) {
            continue;
        }
        if (channel->callback != NULL) {
            channel->callback(channel, VOICE_STOLEN, channel->callbackArg);
        }
        SndHw_StopChannel(i, 0);
        channel->priority = 0;
        func_arm7_037ff68c(channel);
        channel->sync = 0;
        channel->active = FALSE;
    }
}
