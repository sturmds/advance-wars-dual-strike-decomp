#include "types.h"
#include "game/ai_unit_action.h"

/* Clears the 0x40 bytes at data_0229e1b0. */
void func_0208d548(void);

void func_0208e89c(void)
{
    gAiQueueCursor = gAiQueue;
    *gAiQueueCursor++ = AI_QUEUE_RUN_ONCE;
    *gAiQueueCursor = 0;
    gAiQueueCursor = gAiQueue;
    gAiQueueRoutine = func_0208d548;
    gAiTurnState->state = AI_STATE_RUN_QUEUE;
}
