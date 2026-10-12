#include "sdk/snd.h"

void SndPlayer_UpdateChannels(SndPlayer *player)
{
    int i;

    for (i = 0; i < SND_TRACK_COUNT; i++) {
        SndTrack *track = SndPlayer_GetTrack(player, i);
        if (track != NULL) {
            SndTrack_UpdateChannels(track, player, TRUE);
        }
    }
}
