#include "types.h"
#include "sdk/os.h"

void OS_SleepThread(OSThreadQueue *queue);
void OS_WakeupThread(OSThreadQueue *queue);

/* Puts a message at the back of the queue. With OS_MESSAGE_BLOCK it waits for room. */
BOOL OS_SendMessage(OSMessageQueue *queue, OSMessage msg, int flags)
{
    int irqState = OS_DisableInterrupts();

    while (queue->msgCount <= queue->usedCount) {
        if ((flags & OS_MESSAGE_BLOCK) == 0) {
            OS_RestoreInterrupts(irqState);
            return FALSE;
        }
        OS_SleepThread((OSThreadQueue *)&queue->queueSend);
    }
    queue->msgArray[(queue->firstIndex + queue->usedCount) % queue->msgCount] = msg;
    queue->usedCount++;
    OS_WakeupThread((OSThreadQueue *)&queue->queueReceive);
    OS_RestoreInterrupts(irqState);
    return TRUE;
}
