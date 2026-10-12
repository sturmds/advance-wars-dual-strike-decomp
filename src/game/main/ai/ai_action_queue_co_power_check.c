#include "types.h"
#include "game/ai_unit_action.h"

/* Decides whether the AI uses a CO Power or Super CO Power (calls SuperPowerCost). */
void AiPlayer_TryUsePower(void);

void AiAction_QueueCoPowerCheck(void) {
  gAiQueueCursor = gAiQueue;
  *gAiQueueCursor++ = AI_QUEUE_RUN_ONCE;
  *gAiQueueCursor = 0;
  gAiQueueCursor = gAiQueue;
  gAiQueueRoutine = AiPlayer_TryUsePower;
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
}
