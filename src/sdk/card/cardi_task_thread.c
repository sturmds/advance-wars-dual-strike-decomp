/* needed to match: the original was built without copy propagation */
#pragma opt_propagation off
#include "sdk/card.h"

void OS_SleepThread(OSThreadQueue *queue);

/* Card thread: sleeps until a task is queued, then runs it. */
void CARDi_TaskThread(void *arg)
{
    CARDiCommon *common;
    int state;
    OSThread *thread;
    OSThreadQueue *noQueue;
    common = &data_022abb00;
    noQueue = NULL;
    thread = &common->thread;

    while (TRUE) {
        state = OS_DisableInterrupts();

        while (TRUE) {
            if (common->flag & CARD_STAT_TASK) {
                break;
            }
            common->waiter = thread;
            OS_SleepThread(noQueue);
        }
        OS_RestoreInterrupts(state);
        common->task(common);
    }
}
