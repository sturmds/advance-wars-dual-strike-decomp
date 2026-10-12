#include "sdk/snd_seq.h"

SndTrack *func_arm7_03800ea4(SndPlayer *player, int trackNo);
void SndTrack_ReleaseChannels(SndTrack *track, SndPlayer *player, int release);
void SndTrack_FreeChannels(SndTrack *track);

/* Mutes or unmutes the selected tracks (bit i of trackMask is track i). Muting also releases their notes. */
void SndArm7_SetTrackMute(int playerNo, u32 trackMask, int mute)
{
    SndPlayer *player = &data_arm7_038085d0[playerNo];
    SndTrack *track;
    int i;

    for (i = 0; i < SND_TRACK_COUNT && trackMask != 0; i++, trackMask >>= 1) {
        if (trackMask & 1) {
            track = func_arm7_03800ea4(player, i);
            if (track != NULL) {
                if (mute != 0) {
                    SndTrack_ReleaseChannels(track, player, 127);
                    SndTrack_FreeChannels(track);
                }
                track->mute = (u8)mute;
            }
        }
    }
}
