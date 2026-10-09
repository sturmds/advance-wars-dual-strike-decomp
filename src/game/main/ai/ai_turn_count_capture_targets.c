#include "types.h"
#include "game/ai_unit_action.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

BOOL Player_IsInGame(u8 player);
/* 1 + the properties the player owns (bases, cities, airports, ports, Com Towers). */
int func_0208d8b4(int player);

/* Totals func_0208d8b4 over the neutral player and every player in the game
 * on another team. The name is a guess from what it counts. */
void AiTurn_CountCaptureTargets(void)
{
    int player;

    gAiTurnState->captureTargets = 0;
    gAiTurnState->captureTargets += func_0208d8b4(0);
    for (player = 1; player < 5; player++) {
        if (Player_IsInGame(player) && GAME->players[GAME->currentPlayer].team != GAME->players[player].team) {
            gAiTurnState->captureTargets += func_0208d8b4(player);
        }
    }
}
