#include "types.h"
#include "game/Unk_02080440.h"
#include "game/player.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

BOOL Player_IsInGame(u8 player);
int AiTurn_IsPhase3(void);

/* TRUE when mode is 2 and AiTurn_IsPhase3 says so, or when the current player
 * has 30000 funds or more and a player on another team still in the game has
 * more front CO power charge. Nothing calls it. */
BOOL func_020990c4(int mode)
{
    int player;

    if (mode == 2 && AiTurn_IsPhase3() == 1) {
        return TRUE;
    }
    if ((u32)STATE->players[STATE->currentPlayer].funds >= 30000) {
        for (player = 1; player < 5; player++) {
            if (!Player_IsInGame(player)) {
                continue;
            }
            if (STATE->players[STATE->currentPlayer].team == STATE->players[player].team) {
                continue;
            }
            if (STATE->players[player].front.charge > STATE->players[STATE->currentPlayer].front.charge) {
                return TRUE;
            }
        }
    }
    return FALSE;
}
