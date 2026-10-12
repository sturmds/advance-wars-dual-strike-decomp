#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

BOOL AiCo_FrontCloserToSuper(void);
void AiPlayer_PerformAction(int action);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* AiTurn_Update state 5, run after the unit actions; switches on
 * Player.tagPowerHalf (guess: the AI's CO power step). With no tag power
 * on, AiCo_FrontCloserToSuper decides whether to perform action 15 and the
 * turn moves on to state 6. While the front half is on, action 15 runs again and the
 * state stays 5. */
void AiTurn_RunPowerStep(void)
{
    switch (STATE->players[STATE->currentPlayer].tagPowerHalf) {
    case 0:
        if (AiCo_FrontCloserToSuper() == TRUE) {
            AiPlayer_PerformAction(15);
        }
        gAiTurnState->state = 6;
        break;
    case 1:
        AiPlayer_PerformAction(15);
        break;
    case 2:
        gAiTurnState->state = 6;
        break;
    default:
        gAiTurnState->state = 6;
        break;
    }
}
