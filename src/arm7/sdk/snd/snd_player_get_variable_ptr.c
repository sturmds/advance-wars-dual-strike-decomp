#include "sdk/snd.h"

/* Sequence variables 0-15 belong to the player, 16 and up are shared by all players. */
#define SEQ_LOCAL_VAR_COUNT 16

s16 *SndPlayer_GetVariablePtr(SndPlayer *player, int varNo)
{
    SndSharedWork *work = data_arm7_0380808c;

    if (work == NULL) {
        return NULL;
    }
    if (varNo < SEQ_LOCAL_VAR_COUNT) {
        return &work->player[player->number].variable[varNo];
    } else {
        return &work->globalVariable[varNo - SEQ_LOCAL_VAR_COUNT];
    }
}
