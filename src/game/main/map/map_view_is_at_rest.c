#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

/* TRUE when no view scroll is in progress and the cursor sits on a whole
 * tile (cursorPixelX and cursorPixelY have their low 4 bits clear). */
BOOL MapView_IsAtRest(void)
{
    Unk_02080440_48 *state = GAME;

    if ((state->unk_1c4->scrollFlags & 15) == 0 && ((s16)state->cursorPixelX & 15) == 0 && ((s16)state->cursorPixelY & 15) == 0) {
        return TRUE;
    }
    return FALSE;
}
