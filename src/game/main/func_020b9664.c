#include "types.h"
#include "game/Unk_02080440.h"
#include "game/player.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern u32 data_0229eaec;

int func_0207bd6c(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* FALSE when data_0229eaec is 0. Otherwise TRUE when func_0207bd6c says so
 * or the current player is not an AI player. */
BOOL func_020b9664(void)
{
    if (data_0229eaec == 0) {
        return FALSE;
    }
    if (func_0207bd6c() != 0) {
        return TRUE;
    }
    return STATE->slotControllers[STATE->currentPlayer] != PLAYER_CONTROLLER_AI;
}
