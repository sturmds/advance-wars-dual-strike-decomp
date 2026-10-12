#include "types.h"
#include "sdk/os.h"

void OS_SleepThread(OSThreadQueue* queue);
void OS_WakeupThread(OSThreadQueue* queue);

/* Puts a message at the front of the queue, so it is received next. */
BOOL OS_JamMessage(OSMessageQueue* queue, OSMessage msg, int flags) {
  int irqState = OS_DisableInterrupts();

  while (queue->msgCount <= queue->usedCount) {
    if ((flags & OS_MESSAGE_BLOCK) == 0) {
      OS_RestoreInterrupts(irqState);
      return FALSE;
    }
    OS_SleepThread((OSThreadQueue*)&queue->queueSend);
  }
  queue->firstIndex =
      (queue->firstIndex + queue->msgCount - 1) % queue->msgCount;
  queue->msgArray[queue->firstIndex] = msg;
  queue->usedCount++;
  OS_WakeupThread((OSThreadQueue*)&queue->queueReceive);
  OS_RestoreInterrupts(irqState);
  return TRUE;
}
