#include "sdk/snd.h"
#include "sdk/snd_seq.h"

void func_arm7_03800df4(SndPlayer *player);

void SndArm7_StopSeq(int playerNo)
{
    SndPlayer *player = &data_arm7_038085d0[playerNo];

    if (player->active) {
        func_arm7_03800df4(player);
        if (data_arm7_0380808c != NULL) {
            data_arm7_0380808c->playerStatus &= ~(1 << playerNo);
        }
    }
}
