#include "sdk/snd_seq.h"

void SndPlayer_Stop(SndPlayer *player);
SndTrack *SndPlayer_GetTrack(SndPlayer *player, int trackNo);

/* Stops every playing sequence with a track reading from [start, end]. */
void SndArm7_InvalidateSeq(u32 start, u32 end)
{
    SndPlayer *player;
    SndTrack *track;
    int i;
    int j;

    for (i = 0; i < SND_PLAYER_COUNT; i++) {
        player = &data_arm7_038085d0[i];

        if (player->active) {
            for (j = 0; j < SND_TRACK_COUNT; j++) {
                track = SndPlayer_GetTrack(player, j);
                if (track != NULL && start <= (u32)track->cursor && (u32)track->cursor <= end) {
                    SndPlayer_Stop(player);
                    break;
                }
            }
        }
    }
}
