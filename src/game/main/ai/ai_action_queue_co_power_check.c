#include "types.h"
#include "game/ai_unit_action.h"

/* Decides whether the AI uses a CO Power or Super CO Power (calls SuperPowerCost). */
void func_020995c4(void);

void AiAction_QueueCoPowerCheck(void)
{
    data_0229e168 = data_0229e1f0;
    *data_0229e168++ = AI_QUEUE_RUN_ONCE;
    *data_0229e168 = 0;
    data_0229e168 = data_0229e1f0;
    data_0229e170 = func_020995c4;
    *data_021619c8 = AI_STATE_RUN_QUEUE;
}
