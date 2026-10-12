#include "sdk/snd_seq.h"

void func_arm7_03800df4(SndPlayer *player);

/* Stops every playing sequence whose bank lies in [start, end]. */
void SndArm7_InvalidateBank(u32 start, u32 end)
{
    int i;

    for (i = 0; i < SND_PLAYER_COUNT; i++) {
        SndPlayer *player = &data_arm7_038085d0[i];

        if (player->active && start <= player->bank && player->bank <= end) {
            func_arm7_03800df4(player);
        }
    }
}
