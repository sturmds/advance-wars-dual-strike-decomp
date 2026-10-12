#include "sdk/snd_seq.h"

void func_arm7_037ff68c(SndExChannel *channel);

void SndTrack_FreeChannels(SndTrack *track)
{
    SndExChannel *channel;

    for (channel = track->channels; channel != NULL; channel = channel->next) {
        func_arm7_037ff68c(channel);
    }
    track->channels = NULL;
}
