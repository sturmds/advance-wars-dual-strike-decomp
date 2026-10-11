#include "sdk/card.h"

extern OSThreadInfo OSi_ThreadInfo;

u32 func_0210d354(OSThread *thread);   /* returns the thread's priority */
void OS_SetThreadPriority(OSThread *thread, u32 priority);
void OS_WakeupThreadDirect(OSThread *thread);

void CARDi_SetTask(GameCardTaskStep task)
{
    CARDiCommon *common = &data_022abb00;
    u32 priority = common->priorityMode;

    switch (priority) {
    case GAMECARD_PRIO_OUTRANK_CALLER:
        priority = func_0210d354(OSi_ThreadInfo.current);
        if (priority != 0) {
            priority--;
        }
        break;
    case GAMECARD_PRIO_YIELD_TO_CALLER:
        priority = func_0210d354(OSi_ThreadInfo.current);
        if (priority < 31) {
            priority++;
        }
        break;
    }
    OS_SetThreadPriority(&common->thread, priority);
    common->waiter = &common->thread;
    common->task = task;
    common->flag |= CARD_STAT_TASK;
    OS_WakeupThreadDirect(&common->thread);
}
