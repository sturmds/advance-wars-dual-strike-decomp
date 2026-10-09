#include "types.h"
#include "game/sound.h"

#define SEQ_PLAYER_COUNT 16

extern SoundSeqPlayer data_022a8068[SEQ_PLAYER_COUNT];   /* NitroSystem sequence players */

void func_021053a0(SoundSeqPlayer *player, int fadeFrames);

/* NitroSystem NNS_SndPlayerStopSeqAll: stops every sequence player that is in use. */
void NNS_SndPlayerStopSeqAll(int fadeFrames)
{
    int i;

    for (i = 0; i < SEQ_PLAYER_COUNT; i++) {
        if (data_022a8068[i].state != SEQ_PLAYER_FREE) {
            func_021053a0(&data_022a8068[i], fadeFrames);
        }
    }
}
