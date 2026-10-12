#include "sdk/snd.h"

SndTrack *func_arm7_03800ea4(SndPlayer *player, int trackNo);
void func_arm7_03800c4c(SndTrack *track, SndPlayer *player, BOOL release);

void SndPlayer_UpdateChannels(SndPlayer *player)
{
    int i;

    for (i = 0; i < SND_TRACK_COUNT; i++) {
        SndTrack *track = func_arm7_03800ea4(player, i);
        if (track != NULL) {
            func_arm7_03800c4c(track, player, TRUE);
        }
    }
}
