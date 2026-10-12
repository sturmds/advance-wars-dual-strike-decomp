#include "types.h"
#include "sdk/os_arm7.h"

int func_arm7_037fdb80(void);
void func_arm7_037fdb94(int state);
void func_arm7_037fbe2c(OSThreadQueue *queue);
void func_arm7_037fbe9c(OSThreadQueue *queue);

/*
 * ARM7 copy of OS_ReceiveMessage; main already has that symbol. Takes the
 * oldest message. With OS_MESSAGE_BLOCK it waits for one to arrive.
 */
BOOL func_arm7_037fc3ac(OSMessageQueue *queue, OSMessage *msg, int flags)
{
    int irqState = func_arm7_037fdb80();
    int block = flags & OS_MESSAGE_BLOCK;

    while (queue->usedCount == 0) {
        if (block == 0) {
            func_arm7_037fdb94(irqState);
            return FALSE;
        }
        func_arm7_037fbe9c((OSThreadQueue *)&queue->queueReceive);
    }
    if (msg != NULL) {
        *msg = queue->msgArray[queue->firstIndex];
    }
    queue->firstIndex = ARM7_REMAINDER(queue->firstIndex + 1, queue->msgCount);
    queue->usedCount--;
    func_arm7_037fbe2c((OSThreadQueue *)queue);
    func_arm7_037fdb94(irqState);
    return TRUE;
}
