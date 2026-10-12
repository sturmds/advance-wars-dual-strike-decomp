#include "sdk/wmsp.h"

/* Needed to match: the early returns get their own epilogues, as -O4,p builds them. */
#pragma optimize_for_size off

/*
 * Takes an entry from the free list, fills it with the send request and appends
 * it to the ready queue of priority prio. Ports 8 to 15 add a 2-byte sequence
 * number, which counts against the size limit in WMStatus.unk_18e.
 */
int WMSP_QueuePortSend(u16 childBitmap, u16 prio, u16 port, u32 destBitmap,
                       const u16* data, u16 size, void (*callback)(void* arg),
                       void* arg) {
  WMStatus* status = func_arm7_027c05e0()->buf->status;
  RadioOutboxEntry* queueData = status->sendQueueData;
  RadioOutboxList* queue = &status->readyQueue[prio];
  RadioOutboxEntry* entry;
  u16 index;

  if (size == 0) {
    return WM_ERRCODE_INVALID_PARAM;
  }
  if (size + ((port & 8) ? 2 : 0) > status->unk_18e) {
    return WM_ERRCODE_INVALID_PARAM;
  }
  Mutex_Take_Arm7(&status->sendQueueMutex);
  index = status->sendQueueFreeList.head;
  if (index == WIRELESS_OUTBOX_NO_ENTRY) {
    Mutex_Release_Arm7(&status->sendQueueMutex);
    return WM_ERRCODE_SEND_QUEUE_FULL;
  }
  entry = &queueData[index];
  status->sendQueueFreeList.head = entry->next;
  if (status->sendQueueFreeList.tail == index) {
    status->sendQueueFreeList.tail = WIRELESS_OUTBOX_NO_ENTRY;
  }
  entry->port = port;
  entry->destBitmap = destBitmap;
  entry->restBitmap = destBitmap & childBitmap;
  entry->sentBitmap = 0;
  entry->unk_0a = 0;
  entry->data = data;
  entry->size = size;
  entry->callback = callback;
  entry->arg = arg;
  entry->next = WIRELESS_OUTBOX_NO_ENTRY;
  entry->seqNo = WIRELESS_OUTBOX_NO_ENTRY;
  entry->retryCount = status->unk_98;
  if (queue->tail == WIRELESS_OUTBOX_NO_ENTRY) {
    queue->head = index;
  } else {
    queueData[queue->tail].next = index;
  }
  queue->tail = index;
  Mutex_Release_Arm7(&status->sendQueueMutex);
  return WM_ERRCODE_OPERATING;
}
