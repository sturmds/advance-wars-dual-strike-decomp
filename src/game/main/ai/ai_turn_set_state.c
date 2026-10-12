#include "types.h"
#include "game/ai_unit_action.h"

/* Sets gAiTurnState->state (AI_STATE_*). */
void AiTurn_SetState(int state) { gAiTurnState->state = state; }
