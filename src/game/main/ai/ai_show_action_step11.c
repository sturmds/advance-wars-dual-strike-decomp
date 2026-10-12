#include "types.h"
#include "game/ai_unit_action.h"

/* Last display step: back to running the queue. */
void AiShowAction_Step11(void) {
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
  gAiTurnState->actionStep = 0;
}
