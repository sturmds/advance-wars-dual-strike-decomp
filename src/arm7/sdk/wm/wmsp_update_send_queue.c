#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/wm.h"
#include "sdk/wmsp.h"

/* Needed to match: the early returns get their own epilogues, as -O4,p builds them. */
#pragma optimize_for_size off

/*
 * Runs after an MP exchange. Each entry of the send queues was offered to
 * the stations in its sendingBitmap; unless the exchange failed, the ones
 * outside missedBitmap now have the data. An entry that still has stations
 * to reach and may be resent goes back to the front of its ready queue; any
 * other entry is reported to the ARM9 (PORT_SEND) and freed. Returns TRUE
 * if any entry is to be resent.
 */
BOOL WMSP_UpdateSendQueue(BOOL failed, u16 missedBitmap)
{
    /* Needed to match: declared apart from the assignments, in this order. */
    int prio;
    u16 stationBitmap;
    WMStatus *status;
    RadioOutboxEntry *queueData;
    BOOL resend;
    BOOL isParent;
    int index;
    RadioOutboxList *queue;

    status = func_arm7_027c05e0()->buf->status;
    queueData = status->sendQueueData;
    resend = FALSE;

    if (status->state == WM_STATE_MP_PARENT) {
        isParent = TRUE;
    } else if (status->state == WM_STATE_MP_CHILD) {
        isParent = FALSE;
    } else {
        return resend;
    }

    Mutex_Take_Arm7(&status->sendQueueMutex);
    if (status->unk_72c == 0) {
        Mutex_Release_Arm7(&status->sendQueueMutex);
        return FALSE;
    }

    /* A child only sends to the parent. */
    if (!isParent) {
        stationBitmap = 1;
    } else {
        int irqState = Cpu_MaskIrq_Arm7();
        stationBitmap = status->childBitmap;
        Cpu_RestoreIrqBit_Arm7(irqState);
    }

    for (prio = 0, queue = status->sendQueue; prio < WIRELESS_URGENCY_LEVELS; queue++, prio++) {
        RadioOutboxEntry *entry;
        u16 resendTail = WIRELESS_OUTBOX_NO_ENTRY;
        u16 resendHead = WIRELESS_OUTBOX_NO_ENTRY;

        while ((index = queue->head) != WIRELESS_OUTBOX_NO_ENTRY) {
            entry = &queueData[index];

            if (!failed) {
                entry->sentBitmap |= entry->unk_0a & ~missedBitmap;
                entry->restBitmap &= ~entry->sentBitmap;
            }
            entry->restBitmap &= stationBitmap;
            entry->unk_0a = 0;

            if (entry->restBitmap != 0 && ((entry->port & 8) || entry->unk_12 != 0)) {
                resend = TRUE;
                if (entry->unk_12 != 0) {
                    entry->unk_12--;
                }
                if (entry->next == WIRELESS_OUTBOX_NO_ENTRY) {
                    queue->tail = WIRELESS_OUTBOX_NO_ENTRY;
                }
                queue->head = entry->next;
                entry->next = WIRELESS_OUTBOX_NO_ENTRY;
                if (resendTail == WIRELESS_OUTBOX_NO_ENTRY) {
                    resendHead = index;
                } else {
                    queueData[resendTail].next = index;
                }
                resendTail = index;
            } else {
                WMPortSendCallback *reply = Wmsp_GetCallbackBuffer();

                reply->apiid = WM_APIID_PORT_SEND;
                if (entry->restBitmap == 0) {
                    reply->errcode = WM_ERRCODE_SUCCESS;
                } else {
                    reply->errcode = WM_ERRCODE_SEND_FAILED;
                }
                reply->state = WM_STATECODE_PORT_SEND;
                reply->port = entry->port;
                reply->destBitmap = entry->destBitmap;
                reply->restBitmap = entry->restBitmap;
                reply->sentBitmap = entry->sentBitmap;
                reply->size = entry->size;
                reply->data = entry->data;
                reply->callback = entry->callback;
                reply->arg = entry->arg;
                reply->seqNo = entry->seqNo;
                Wmsp_SendReply(reply);

                if (entry->next == WIRELESS_OUTBOX_NO_ENTRY) {
                    queue->tail = WIRELESS_OUTBOX_NO_ENTRY;
                }
                queue->head = entry->next;
                entry->next = WIRELESS_OUTBOX_NO_ENTRY;
                if (status->sendQueueFreeList.tail == WIRELESS_OUTBOX_NO_ENTRY) {
                    status->sendQueueFreeList.head = index;
                } else {
                    queueData[status->sendQueueFreeList.tail].next = index;
                }
                status->sendQueueFreeList.tail = index;
            }
        }

        if (resendTail != WIRELESS_OUTBOX_NO_ENTRY) {
            queueData[resendTail].next = status->readyQueue[prio].head;
            if (status->readyQueue[prio].tail == WIRELESS_OUTBOX_NO_ENTRY) {
                status->readyQueue[prio].tail = resendTail;
            }
            status->readyQueue[prio].head = resendHead;
        }
    }

    status->unk_72c = 0;
    Mutex_Release_Arm7(&status->sendQueueMutex);
    return resend;
}
