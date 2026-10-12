#include "types.h"
#include "game/ai_unit_action.h"

void AiTurn_BuildCaptureMap(void);

void func_0208e8fc(void) {
  gAiQueueCursor = gAiQueue;
  *gAiQueueCursor++ = AI_QUEUE_RUN_ONCE;
  *gAiQueueCursor = 0;
  gAiQueueCursor = gAiQueue;
  gAiQueueRoutine = AiTurn_BuildCaptureMap;
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
}
