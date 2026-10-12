#include "types.h"
#include "game/ai_unit_action.h"

void AiTurn_ReleaseCaptureClaims(void);

void func_0208e77c(void) {
  gAiQueueCursor = gAiQueue;
  *gAiQueueCursor++ = AI_QUEUE_RUN_ONCE;
  *gAiQueueCursor = 0;
  gAiQueueCursor = gAiQueue;
  gAiQueueRoutine = AiTurn_ReleaseCaptureClaims;
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
}
