/* needed to match: the original was built without copy propagation */
#pragma opt_propagation off
#include "types.h"
#include "sdk/card_arm7.h"

int func_arm7_037fdb80(void);
void func_arm7_037fdb94(int state);
void Thread_ReadyAllWaiters_Arm7(CARDiArm7ThreadQueue *queue);
void Thread_MakeReady_Arm7(void *thread);

/* Ends the running request: reports success, wakes the waiters and calls the callback. */
void Card_ReportDoneAndWake(void)
{
    CARDiArm7Common *common = &data_arm7_03809280;
    CARDCallback callback = common->callback;
    void *callbackArg = common->callbackArg;
    int interrupts;

    common->cmd->result = CARD_RESULT_SUCCESS;
    interrupts = func_arm7_037fdb80();
    common->flag &= ~(CARD_STAT_BUSY | CARD_STAT_TASK);
    Thread_ReadyAllWaiters_Arm7(&common->queue);
    if (common->flag & GAMECARD_FLAG_WORKER_SLEEPS) {
        Thread_MakeReady_Arm7(&common->thread);
    }
    func_arm7_037fdb94(interrupts);
    if (callback != NULL) {
        callback(callbackArg);
    }
}
