#include "sdk/snd.h"

/* The tempo counter gains tempo * tempoRatio / 256 per call; each 240 in it is one sequence tick. */
#define SEQ_BEAT_ACCUM_STEP 240

void SndPlayer_Advance(SndPlayer *player)
{
    int ticks = 0;
    int i;
    int step;

    while (player->tempoCounter >= SEQ_BEAT_ACCUM_STEP) {
        player->tempoCounter -= SEQ_BEAT_ACCUM_STEP;
        ticks++;
    }
    for (i = 0; i < ticks; i++) {
        if (SndPlayer_StepTracks(player, TRUE)) {
            SndPlayer_Stop(player);
            break;
        }
    }
    if (data_arm7_0380808c != NULL) {
        data_arm7_0380808c->player[player->number].tickCounter += i;
    }
    step = player->tempo;
    step *= player->tempoRatio;
    player->tempoCounter += step >> 8;
}
