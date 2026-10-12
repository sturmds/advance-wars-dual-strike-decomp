#include "types.h"
#include "sdk/os_arm7.h"

/*
 * ARM7 copy of OS_SendMessage; main already has that symbol. Puts a message
 * at the back of the queue. With OS_MESSAGE_BLOCK it waits for room.
 */
BOOL MsgQueue_PushBack_Arm7(OSMessageQueue* queue, OSMessage msg, int flags) {
  int irqState = Cpu_MaskIrq_Arm7();
  int block = flags & OS_MESSAGE_BLOCK;

  while (queue->msgCount <= queue->usedCount) {
    if (block == 0) {
      Cpu_RestoreIrqBit_Arm7(irqState);
      return FALSE;
    }
    Thread_WaitOnQueue_Arm7((OSThreadQueue*)queue);
  }
  queue->msgArray[(queue->firstIndex + queue->usedCount) % queue->msgCount] =
      msg;
  queue->usedCount++;
  Thread_ReadyAllWaiters_Arm7((OSThreadQueue*)&queue->queueReceive);
  Cpu_RestoreIrqBit_Arm7(irqState);
  return TRUE;
}
