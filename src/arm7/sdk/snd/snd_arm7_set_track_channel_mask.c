#include "sdk/snd_seq.h"

SndTrack *func_arm7_03800ea4(SndPlayer *player, int trackNo);

/* Limits the selected tracks (bit i of trackMask is track i) to the hardware channels in channelMask. */
void SndArm7_SetTrackChannelMask(int playerNo, u32 trackMask, u32 channelMask)
{
    SndPlayer *player = &data_arm7_038085d0[playerNo];
    SndTrack *track;
    int i;

    for (i = 0; i < SND_TRACK_COUNT && trackMask != 0; i++, trackMask >>= 1) {
        if (trackMask & 1) {
            track = func_arm7_03800ea4(player, i);
            if (track != NULL) {
                track->channelMask = channelMask;
                track->channelMaskSet = 1;
            }
        }
    }
}
