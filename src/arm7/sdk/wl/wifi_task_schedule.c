#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/wl.h"

#define WIFI_TASKQ_LOCK_IRQS 0x01000010
#define WIFI_TASK_LEVEL_IDLE 3

BOOL MsgQueue_PushBack_Arm7(OSMessageQueue *queue, OSMessage message, int flags); /* OS_SendMessage */
u32 Irq_ClearEnableBits_Arm7(u32 mask); /* ARM7 OS_DisableIrqMask */
u32 Irq_SetEnableBits_Arm7(u32 mask); /* ARM7 OS_EnableIrqMask */

/* Puts a task at the back of its priority queue, unless it is queued already. */
void WifiTask_Schedule(int priority, int task)
{
    WlWork *work = WIFI_DRIVER_STATE;
    WlTask *tasks = work->tasks;
    u32 irqState;

    irqState = Irq_ClearEnableBits_Arm7(WIFI_TASKQ_LOCK_IRQS);
    if (tasks[task].queued == 0) {
        tasks[task].queued = 1;
        tasks[task].next = WIFI_TASK_LIST_END;
        if (work->queueTail[priority] == WIFI_TASK_LIST_END) {
            work->queueHead[priority] = task;
        } else {
            work->tasks[work->queueTail[priority]].next = task;
        }
        work->queueTail[priority] = task;
        if (priority < work->topPriority) {
            work->topPriority = priority;
        }
    }
    Irq_SetEnableBits_Arm7(irqState);
    if (priority != WIFI_TASK_LEVEL_IDLE && work->runPriority == WIFI_TASK_LEVEL_IDLE) {
        MsgQueue_PushBack_Arm7(WIFI_DRIVER_STATE->limits.fromHost, NULL, OS_MESSAGE_NOBLOCK);
    }
}
