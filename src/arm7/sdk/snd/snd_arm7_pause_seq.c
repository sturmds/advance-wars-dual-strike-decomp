#include "sdk/snd.h"
#include "sdk/snd_seq.h"

SndTrack *func_arm7_03800ea4(SndPlayer *player, int trackIndex);
void SndTrack_ReleaseChannels(SndTrack *track, SndPlayer *player, int release);
void SndTrack_FreeChannels(SndTrack *track);

/* Pausing releases every channel of the player's tracks. The field named prepared
 * holds the pause flag: SndArm7_SeqMain skips the sequence step while it is set. */
void SndArm7_PauseSeq(int playerNo, BOOL pause)
{
    SndPlayer *player = &data_arm7_038085d0[playerNo];
    int i;

    player->prepared = pause;
    if (pause) {
        for (i = 0; i < SND_TRACK_COUNT; i++) {
            SndTrack *track = func_arm7_03800ea4(player, i);
            if (track != NULL) {
                SndTrack_ReleaseChannels(track, player, 127);
                SndTrack_FreeChannels(track);
            }
        }
    }
}
