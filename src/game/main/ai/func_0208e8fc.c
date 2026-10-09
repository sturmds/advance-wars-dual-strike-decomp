#include "types.h"
#include "game/ai_unit_action.h"

void func_0208c560(void);

void func_0208e8fc(void)
{
    gAiQueueCursor = gAiQueue;
    *gAiQueueCursor++ = AI_QUEUE_RUN_ONCE;
    *gAiQueueCursor = 0;
    gAiQueueCursor = gAiQueue;
    gAiQueueRoutine = func_0208c560;
    *gAiTurnState = AI_STATE_RUN_QUEUE;
}
