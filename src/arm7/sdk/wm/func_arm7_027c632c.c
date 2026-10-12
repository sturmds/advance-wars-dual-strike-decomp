#include "sdk/wmsp.h"

/* Built for speed like func_arm7_027c70f8: the match is closer with this pragma. */
#pragma optimize_for_size off

/*
 * Removes the children in aidBitmap from every queued port send. An entry left with
 * no child to reach goes back to the free list with a PORT_SEND reply. The original
 * walks the ready queues twice.
 */
void func_arm7_027c632c(u16 aidBitmap)
{
    RadioOutboxEntry *queueData;
    u16 prev;
    int i;
    u16 index;
    int priority;
    u32 keep;
    RadioOutboxList *queue;
    u16 *link;
    WMStatus *status;
    RadioOutboxEntry *entry;
    RadioOutboxList *queues;
    WMPortSendCallback *callback;

    status = func_arm7_027c05e0()->buf->status;
    queueData = status->sendQueueData;
    keep = ~aidBitmap & status->childBitmap;

    Mutex_Take_Arm7(&status->sendQueueMutex);
    for (i = 0; i < 2; i++) {
        queues = status->readyQueue;
        for (priority = 0; priority < WIRELESS_URGENCY_LEVELS; priority++) {
            queue = &queues[priority];
            link = &queue->head;
            prev = WIRELESS_OUTBOX_NO_ENTRY;
            index = queue->head;
            while (index != WIRELESS_OUTBOX_NO_ENTRY) {
                entry = &queueData[index];
                entry->restBitmap &= keep;
                entry->unk_0a &= keep;
                if (entry->restBitmap == 0) {
                    callback = Wmsp_GetCallbackBuffer();
                    callback->apiid = WM_APIID_PORT_SEND;
                    callback->errcode = WM_ERRCODE_SUCCESS;
                    callback->state = WM_STATECODE_PORT_SEND;
                    callback->port = entry->port;
                    callback->destBitmap = entry->destBitmap;
                    callback->restBitmap = entry->restBitmap;
                    callback->sentBitmap = entry->sentBitmap;
                    callback->size = entry->size;
                    callback->data = entry->data;
                    callback->callback = entry->callback;
                    callback->arg = entry->arg;
                    callback->seqNo = entry->seqNo;
                    Wmsp_SendReply(callback);

                    if (entry->next == WIRELESS_OUTBOX_NO_ENTRY) {
                        queue->tail = prev;
                    }
                    *link = entry->next;
                    entry->next = WIRELESS_OUTBOX_NO_ENTRY;
                    if (status->sendQueueFreeList.tail == WIRELESS_OUTBOX_NO_ENTRY) {
                        status->sendQueueFreeList.head = index;
                    } else {
                        queueData[status->sendQueueFreeList.tail].next = index;
                    }
                    status->sendQueueFreeList.tail = index;
                    index = prev;
                }
                link = (index != WIRELESS_OUTBOX_NO_ENTRY) ? &queueData[index].next : &queue->head;
                prev = index;
                index = (index != WIRELESS_OUTBOX_NO_ENTRY) ? queueData[index].next : queue->head;
            }
        }
    }
    Mutex_Release_Arm7(&status->sendQueueMutex);
}
