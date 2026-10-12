#include "sdk/snd.h"

void func_arm7_03800e30(SndPlayer *player, int trackNo);

void SndPlayer_Stop(SndPlayer *player)
{
    int i;

    for (i = 0; i < SND_TRACK_COUNT; i++) {
        func_arm7_03800e30(player, i);
    }
    player->active = FALSE;
}
