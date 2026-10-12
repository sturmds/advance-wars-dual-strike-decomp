#include "types.h"
#include "sdk/os_arm7.h"

void MsgQueue_SetUpRing_Arm7(OSMessageQueue* queue, OSMessage* msgArray,
                             s32 msgCount) {
  queue->queueSend = 0;
  queue->queueReceive = 0;
  queue->msgArray = msgArray;
  queue->msgCount = msgCount;
  queue->firstIndex = 0;
  queue->usedCount = 0;
}
