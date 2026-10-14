#include "types.h"
#include "game/ai_unit_action.h"

extern AiTurnState* gAiTurnState;

int AiTurn_IsPhase3(void) { return (gAiTurnState->phase == 3) ? 1 : 0; }
