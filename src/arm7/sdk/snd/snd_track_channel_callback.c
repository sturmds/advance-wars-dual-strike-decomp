#include "sdk/snd.h"

void func_arm7_037ff68c(SndExChannel *channel);

/* Called by the channel code when a channel of the track stops or is taken by another sound. */
void SndTrack_ChannelCallback(SndExChannel *channel, int status, void *data)
{
    SndTrack *track = data;
    SndExChannel *prev;

    if (status == VOICE_DONE) {
        channel->unk_22 = 0;
        func_arm7_037ff68c(channel);
    }
    if (track->channels == channel) {
        track->channels = channel->next;
        return;
    }
    for (prev = track->channels; prev->next != NULL; prev = prev->next) {
        if (prev->next == channel) {
            prev->next = channel->next;
            return;
        }
    }
}
