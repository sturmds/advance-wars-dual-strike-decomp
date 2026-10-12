#include "types.h"
#include "sdk/os_arm7.h"

int func_arm7_037fdb80(void);
void func_arm7_037fdb94(int state);

/*
 * ARM7 copy of OS_SendMessage; main already has that symbol. Puts a message
 * at the back of the queue. With OS_MESSAGE_BLOCK it waits for room.
 */
BOOL MsgQueue_PushBack_Arm7(OSMessageQueue *queue, OSMessage msg, int flags)
{
    int irqState = func_arm7_037fdb80();
    int block = flags & OS_MESSAGE_BLOCK;

    while (queue->msgCount <= queue->usedCount) {
        if (block == 0) {
            func_arm7_037fdb94(irqState);
            return FALSE;
        }
        Thread_WaitOnQueue_Arm7((OSThreadQueue *)queue);
    }
    queue->msgArray[ARM7_REMAINDER(queue->firstIndex + queue->usedCount, queue->msgCount)] = msg;
    queue->usedCount++;
    Thread_ReadyAllWaiters_Arm7((OSThreadQueue *)&queue->queueReceive);
    func_arm7_037fdb94(irqState);
    return TRUE;
}
