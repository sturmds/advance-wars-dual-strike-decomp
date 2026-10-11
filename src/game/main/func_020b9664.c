#include "types.h"
#include "game/Unk_02080440.h"
#include "game/player.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern u32 gLinkPlay;

int Link_GetOwnPlayer(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* FALSE when gLinkPlay is 0. Otherwise TRUE when Link_GetOwnPlayer says so
 * or the current player is not an AI player. */
BOOL func_020b9664(void)
{
    if (gLinkPlay == 0) {
        return FALSE;
    }
    if (Link_GetOwnPlayer() != 0) {
        return TRUE;
    }
    return STATE->slotControllers[STATE->currentPlayer] != PLAYER_CONTROLLER_AI;
}
