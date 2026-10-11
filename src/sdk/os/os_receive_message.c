#include "types.h"
#include "sdk/os.h"

void OS_SleepThread(OSThreadQueue *queue);
void OS_WakeupThread(OSThreadQueue *queue);

/* Takes the oldest message. With OS_MESSAGE_BLOCK it waits for one to arrive. */
BOOL OS_ReceiveMessage(OSMessageQueue *queue, OSMessage *msg, int flags)
{
    int irqState = OS_DisableInterrupts();

    while (queue->usedCount == 0) {
        if ((flags & OS_MESSAGE_BLOCK) == 0) {
            OS_RestoreInterrupts(irqState);
            return FALSE;
        }
        OS_SleepThread((OSThreadQueue *)&queue->queueReceive);
    }
    if (msg != NULL) {
        *msg = queue->msgArray[queue->firstIndex];
    }
    queue->firstIndex = (queue->firstIndex + 1) % queue->msgCount;
    queue->usedCount--;
    OS_WakeupThread((OSThreadQueue *)&queue->queueSend);
    OS_RestoreInterrupts(irqState);
    return TRUE;
}
