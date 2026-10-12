#include "sdk/snd.h"

/* Runs one tick of every track of the player; play is FALSE when skipping ahead without starting notes. Returns TRUE once all tracks have ended. */
BOOL SndPlayer_StepTracks(SndPlayer *player, int play)
{
    BOOL playing = FALSE;
    int i;

    for (i = 0; i < SND_TRACK_COUNT; i++) {
        SndTrack *track = SndPlayer_GetTrack(player, i);
        if (track != NULL && track->cursor != NULL) {
            if (SndTrack_Step(track, player, i, play) == 0) {
                playing = TRUE;
            } else {
                SndPlayer_CloseTrack(player, i);
            }
        }
    }
    return !playing;
}
