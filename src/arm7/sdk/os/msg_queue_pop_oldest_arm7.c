#include "types.h"
#include "sdk/os_arm7.h"

/*
 * ARM7 copy of OS_ReceiveMessage; main already has that symbol. Takes the
 * oldest message. With OS_MESSAGE_BLOCK it waits for one to arrive.
 */
BOOL MsgQueue_PopOldest_Arm7(OSMessageQueue *queue, OSMessage *msg, int flags)
{
    int irqState = Cpu_MaskIrq_Arm7();
    int block = flags & OS_MESSAGE_BLOCK;

    while (queue->usedCount == 0) {
        if (block == 0) {
            Cpu_RestoreIrqBit_Arm7(irqState);
            return FALSE;
        }
        Thread_WaitOnQueue_Arm7((OSThreadQueue *)&queue->queueReceive);
    }
    if (msg != NULL) {
        *msg = queue->msgArray[queue->firstIndex];
    }
    queue->firstIndex = ARM7_REMAINDER(queue->firstIndex + 1, queue->msgCount);
    queue->usedCount--;
    Thread_ReadyAllWaiters_Arm7((OSThreadQueue *)queue);
    Cpu_RestoreIrqBit_Arm7(irqState);
    return TRUE;
}
