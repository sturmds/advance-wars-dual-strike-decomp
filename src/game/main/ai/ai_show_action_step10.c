#include "types.h"
#include "game/ai_unit_action.h"

extern void *data_0229eacc[2];

int func_020b9914(void);
void UnitShowTask_Destroy(void *task);
void func_020deac4(void);

/* Waits while func_020b9914 returns nonzero, then destroys the two
 * tasks in data_0229eacc, if running, and returns to the queue. */
void AiShowAction_Step10(void)
{
    if (func_020b9914() != 0) {
        return;
    }
    if (data_0229eacc[0] != NULL) {
        UnitShowTask_Destroy(data_0229eacc[0]);
    }
    if (data_0229eacc[1] != NULL) {
        UnitShowTask_Destroy(data_0229eacc[1]);
    }
    func_020deac4();
    gAiTurnState->state = AI_STATE_RUN_QUEUE;
    gAiTurnState->actionStep = 0;
}
