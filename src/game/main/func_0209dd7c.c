#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

BOOL Player_IsInGame(u8 player);
BOOL func_020c0e3c(int player); /* TRUE: bit 0x80 of Player.unk_2f is clear */

#define STATE (data_027c027c->unk_48[data_027c0284])

/* TRUE when some player still in the game, on another team than the
 * current player, passes func_020c0e3c. */
BOOL func_0209dd7c(void)
{
    int player;

    for (player = 1; player < 5; player++) {
        if (Player_IsInGame(player)
            && STATE->players[STATE->currentPlayer].team != STATE->players[player].team
            && func_020c0e3c(player) == TRUE) {
            return TRUE;
        }
    }
    return FALSE;
}
