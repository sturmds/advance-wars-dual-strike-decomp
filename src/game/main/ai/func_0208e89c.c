#include "types.h"
#include "game/ai_unit_action.h"

void Ai_ClearTransportsSentToUnit(void);

void func_0208e89c(void) {
  gAiQueueCursor = gAiQueue;
  *gAiQueueCursor++ = AI_QUEUE_RUN_ONCE;
  *gAiQueueCursor = 0;
  gAiQueueCursor = gAiQueue;
  gAiQueueRoutine = Ai_ClearTransportsSentToUnit;
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
}
