#include "sdk/snd.h"
#include "sdk/snd_seq.h"

void SndArm7_InitSeq(void)
{
    int i;

    for (i = 0; i < SND_PLAYER_COUNT; i++) {
        SndPlayer *player = &data_arm7_038085d0[i];
        player->active = FALSE;
        player->number = i;
    }
    for (i = 0; i < SEQ_TRACKS_SHARED_TOTAL; i++) {
        data_arm7_03808810[i].active = FALSE;
    }
}
