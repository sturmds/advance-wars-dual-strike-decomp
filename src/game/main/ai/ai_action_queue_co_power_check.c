#include "types.h"
#include "game/ai_unit_action.h"

/* Decides whether the AI uses a CO Power or Super CO Power (calls SuperPowerCost). */
void func_020995c4(void);

void AiAction_QueueCoPowerCheck(void)
{
    gAiQueueCursor = gAiQueue;
    *gAiQueueCursor++ = AI_QUEUE_RUN_ONCE;
    *gAiQueueCursor = 0;
    gAiQueueCursor = gAiQueue;
    gAiQueueRoutine = func_020995c4;
    *gAiTurnState = AI_STATE_RUN_QUEUE;
}
