#include "sdk/snd.h"
#include "sdk/snd_seq.h"

extern SndSharedWork *data_arm7_0380808c;

void SndPlayer_Setup(SndPlayer *player, u32 bank)
{
    int i;

    player->prepared = 0;
    player->bank = bank;
    player->tempo = 120;
    player->tempoRatio = 256;
    player->tempoCounter = 240;
    player->volume = 127;
    player->extFader = 0;
    player->priority = 64;
    for (i = 0; i < SND_TRACK_COUNT; i++) {
        player->tracks[i] = 0xFF;
    }
    if (data_arm7_0380808c != NULL) {
        data_arm7_0380808c->player[player->number].tickCounter = 0;
        for (i = 0; i < 16; i++) {
            data_arm7_0380808c->player[player->number].variable[i] = -1;
        }
    }
}
