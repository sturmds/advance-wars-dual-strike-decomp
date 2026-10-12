#include "types.h"
#include "sdk/wl.h"

#define WIFI_TASKQ_LOCK_IRQS 0x01000010

u32 func_arm7_037fb654(u32 mask); /* ARM7 OS_DisableIrqMask */
u32 func_arm7_037fb690(u32 mask); /* ARM7 OS_EnableIrqMask */

/* Takes the first task off a priority queue. Returns its number, or WIFI_TASK_LIST_END if the queue is empty. */
u16 WifiTask_PopNext(int priority)
{
    WlWork *work = WIFI_DRIVER_STATE;
    u32 irqState;
    u16 task;

    irqState = func_arm7_037fb654(WIFI_TASKQ_LOCK_IRQS);
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
    func_arm7_037fb690(irqState);
    return task;
}
