#include "sdk/snd.h"

SndTrack *SndPlayer_GetTrack(SndPlayer *player, int trackNo);
void SndTrack_UpdateChannels(SndTrack *track, SndPlayer *player, BOOL release);

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
