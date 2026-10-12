#include "types.h"
#include "sdk/os_arm7.h"

/*
 * Copies the oldest message without removing it. With OS_MESSAGE_BLOCK it
 * waits for one to arrive.
 */
BOOL MsgQueue_PeekOldest_Arm7(OSMessageQueue* queue, OSMessage* msg,
                              int flags) {
  int irqState = Cpu_MaskIrq_Arm7();
  int block = flags & OS_MESSAGE_BLOCK;

  while (queue->usedCount == 0) {
    if (block == 0) {
      Cpu_RestoreIrqBit_Arm7(irqState);
      return FALSE;
    }
    Thread_WaitOnQueue_Arm7((OSThreadQueue*)&queue->queueReceive);
  }
  if (msg != NULL) {
    *msg = queue->msgArray[queue->firstIndex];
  }
  Cpu_RestoreIrqBit_Arm7(irqState);
  return TRUE;
}
