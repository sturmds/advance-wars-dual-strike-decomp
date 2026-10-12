#include "sdk/snd.h"

void SndPlayer_CloseTrack(SndPlayer *player, int trackNo)
{
    SndTrack *track = SndPlayer_GetTrack(player, trackNo);

    if (track != NULL) {
        SndTrack_Close(track, player);
        data_arm7_03808810[player->tracks[trackNo]].active = FALSE;
        player->tracks[trackNo] = SEQ_TRACK_SLOT_EMPTY;
    }
}
