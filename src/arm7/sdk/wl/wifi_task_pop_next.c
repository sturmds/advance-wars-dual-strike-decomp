#include "types.h"
#include "sdk/wl.h"
#include "sdk/os_arm7.h"

#define WIFI_TASKQ_LOCK_IRQS 0x01000010

/* Takes the first task off a priority queue. Returns its number, or WIFI_TASK_LIST_END if the queue is empty. */
u16 WifiTask_PopNext(int priority)
{
    WlWork *work = WIFI_DRIVER_STATE;
    u32 irqState;
    u16 task;

    irqState = Irq_ClearEnableBits_Arm7(WIFI_TASKQ_LOCK_IRQS);
    task = work->queueHead[priority];
    if (task != WIFI_TASK_LIST_END) {
        WlTask *tasks = work->tasks;

        tasks[task].queued = 0;
        if (tasks[task].next == WIFI_TASK_LIST_END) {
            work->queueHead[priority] = WIFI_TASK_LIST_END;
            work->queueTail[priority] = WIFI_TASK_LIST_END;
        } else {
            work->queueHead[priority] = tasks[task].next;
            tasks[task].next = WIFI_TASK_LIST_END;
        }
    }
    Irq_SetEnableBits_Arm7(irqState);
    return task;
}
