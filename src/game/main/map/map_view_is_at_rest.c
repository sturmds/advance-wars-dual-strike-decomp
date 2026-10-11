#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

/* TRUE when no view scroll is in progress and unk_2c and unk_2e have their
 * low 4 bits clear (a whole tile, if they count in 1/16 tiles like the
 * scroll). What the two fields hold is not known. */
BOOL MapView_IsAtRest(void)
{
    Unk_02080440_48 *state = GAME;

    if ((state->unk_1c4->scrollFlags & 15) == 0 && ((s16)state->unk_2c & 15) == 0 && ((s16)state->unk_2e & 15) == 0) {
        return TRUE;
    }
    return FALSE;
}
