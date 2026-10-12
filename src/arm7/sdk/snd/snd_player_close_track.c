#include "sdk/snd.h"

SndTrack *func_arm7_03800ea4(SndPlayer *player, int trackNo);
int func_arm7_03800e84(SndTrack *track, SndPlayer *player);

void SndPlayer_CloseTrack(SndPlayer *player, int trackNo)
{
    SndTrack *track = func_arm7_03800ea4(player, trackNo);

    if (track != NULL) {
        func_arm7_03800e84(track, player);
        data_arm7_03808810[player->tracks[trackNo]].active = FALSE;
        player->tracks[trackNo] = SEQ_TRACK_SLOT_EMPTY;
    }
}
