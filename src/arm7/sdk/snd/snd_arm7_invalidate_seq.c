#include "sdk/snd_seq.h"

void func_arm7_03800df4(SndPlayer *player);
SndTrack *func_arm7_03800ea4(SndPlayer *player, int trackNo);

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
                track = func_arm7_03800ea4(player, j);
                if (track != NULL && start <= (u32)track->cursor && (u32)track->cursor <= end) {
                    func_arm7_03800df4(player);
                    break;
                }
            }
        }
    }
}
