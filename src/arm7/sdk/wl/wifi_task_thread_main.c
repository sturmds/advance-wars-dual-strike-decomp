#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/wl.h"

#define WIFI_TASKQ_LOCK_IRQS 0x01000010

/* Thread body of the driver: runs the queued tasks, most urgent priority first. */
void WifiTask_ThreadMain(void)
{
    WlWork *work = WIFI_DRIVER_STATE;
    OSMessage message;
    u32 irqState;

    work->topPriority = 0;
    work->runTask = 0;
    while (TRUE) {
        if (MsgQueue_PopOldest_Arm7(WIFI_DRIVER_STATE->limits.fromHost, &message, OS_MESSAGE_NOBLOCK)) {
            WifiHost_TakeInboxBuffer(&message);
        }
        irqState = Irq_ClearEnableBits_Arm7(WIFI_TASKQ_LOCK_IRQS);
        work->runPriority = work->topPriority;
        if (work->queueHead[work->runPriority] == WIFI_TASK_LIST_END) {
            work->topPriority++;
            Irq_SetEnableBits_Arm7(irqState);
            continue;
        }
        Irq_SetEnableBits_Arm7(irqState);
        work->runTask = WifiTask_PopNext(work->runPriority);
        work->tasks[work->runTask].handler();
        work->runTask = WIFI_TASK_LIST_END;
    }
}
