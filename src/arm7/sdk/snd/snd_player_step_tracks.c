#include "sdk/snd.h"

SndTrack *func_arm7_03800ea4(SndPlayer *player, int trackNo);
int func_arm7_03800048(SndTrack *track, SndPlayer *player, int trackNo, int play);
void SndPlayer_CloseTrack(SndPlayer *player, int trackNo);

/* Runs one tick of every track of the player; play is FALSE when skipping ahead without starting notes. Returns TRUE once all tracks have ended. */
BOOL SndPlayer_StepTracks(SndPlayer *player, int play)
{
    BOOL playing = FALSE;
    int i;

    for (i = 0; i < SND_TRACK_COUNT; i++) {
        SndTrack *track = func_arm7_03800ea4(player, i);
        if (track != NULL && track->cursor != NULL) {
            if (func_arm7_03800048(track, player, i, play) == 0) {
                playing = TRUE;
            } else {
                SndPlayer_CloseTrack(player, i);
            }
        }
    }
    return !playing;
}
