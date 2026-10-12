#include "types.h"
#include "game/ai_unit_action.h"

void AiTurn_ClaimCaptureTargets(void);

void func_0208e7dc(void) {
  gAiQueueCursor = gAiQueue;
  *gAiQueueCursor++ = AI_QUEUE_RUN_ONCE;
  *gAiQueueCursor = 0;
  gAiQueueCursor = gAiQueue;
  gAiQueueRoutine = AiTurn_ClaimCaptureTargets;
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
}
