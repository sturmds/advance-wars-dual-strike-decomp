#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/wm.h"
#include "sdk/wmsp.h"
#include "sdk/mi.h"

/* Needed to match: the early returns get their own epilogues, as -O4,p builds them. */
#pragma optimize_for_size off

/*
 * Packs entries of the ready queues, highest priority first, into the data
 * of the next MP frame and moves each packed entry to its send queue. Each
 * entry gets a header word (port in bits 8-11, size in halfwords in bits
 * 0-7), its data, a sequence number on ports 8 to 15, and a destination
 * bitmap (header bit 12) when a parent sends to only some children. The
 * first header goes to *header, later ones into buf. Returns 1 if nothing
 * can be sent now, else 0.
 */
int WMSP_BuildMpSendData(u32 pollBitmap, u32 childBitmap, u32 readyBitmap, int space, u16 *header, u16 *buf,
                       u32 *destBitmapOut, u32 *sizeOut)
{
    WMStatus *status = func_arm7_027c05e0()->buf->status;
    u32 skippedPorts;
    BOOL first;
    BOOL isParent;
    u32 destBitmap;
    u16 *dst;
    u32 size;
    BOOL allPolled;
    u32 targetMask;
    u32 sendBitmap;
    RadioOutboxEntry *queueData;
    u32 prio;
    RadioOutboxList *ready;
    RadioOutboxList *send;

    if (status->aid >= 16) {
        return 1;
    }
    if (status->state == WM_STATE_MP_PARENT) {
        isParent = TRUE;
    } else if (status->state == WM_STATE_MP_CHILD) {
        isParent = FALSE;
    } else {
        return 1;
    }

    destBitmap = 0;
    *header = 0;
    *destBitmapOut = 0;
    *sizeOut = destBitmap;
    size = destBitmap;
    if (space < 0) {
        return 1;
    }

    targetMask = readyBitmap;
    allPolled = TRUE;
    if (isParent) {
        if (status->unk_92 != 0) {
            targetMask = childBitmap;
            allPolled = FALSE;
        }
    } else {
        allPolled = FALSE;
    }
    sendBitmap = readyBitmap & pollBitmap;
    skippedPorts = 0;
    dst = buf;
    first = TRUE;

    Mutex_Take_Arm7(&status->sendQueueMutex);
    if (status->sendQueueInUse == 1) {
        Mutex_Release_Arm7(&status->sendQueueMutex);
        return 1;
    }
    status->sendQueueInUse = 1;
    queueData = status->sendQueueData;

    for (prio = 0, ready = status->readyQueue, send = status->sendQueue; prio < WIRELESS_URGENCY_LEVELS && space > 2;
         ready++, send++, prio++) {
        BOOL withDest;
        u16 *prevNext = &ready->head;
        u16 prev = WIRELESS_OUTBOX_NO_ENTRY;
        u16 index = ready->head;

        while (index != WIRELESS_OUTBOX_NO_ENTRY && space > 2) {
            RadioOutboxEntry *entry = &queueData[index];
            u32 portBit = 1 << entry->port;
            u32 targets;
            BOOL withSeqNo;

            if (skippedPorts & portBit) {
                goto next;
            }
            skippedPorts |= portBit;
            targets = entry->restBitmap & targetMask;
            if (allPolled && (targets & ~sendBitmap)) {
                goto next;
            }
            withSeqNo = (entry->port & 8) ? TRUE : FALSE;
            /*
             * Needed to match: withDest as a ternary and prevNext (below) as
             * an if/else; the other way round swaps their stack slots.
             */
            withDest = (isParent && (targets | 1) != 0xffff) ? TRUE : FALSE;
            if ((entry->size & 1) == 1) {
                entry->size++;
            }
            if (entry->size + (first ? 0 : 2) + (withSeqNo ? 2 : 0) + (withDest ? 2 : 0) > space) {
                goto next;
            }

            if (!first) {
                header = dst;
                *dst++ = 0;
                size += 2;
                space -= 2;
            }
            *header |= ((entry->port << 8) & 0xf00) | (((u32)entry->size >> 1) & 0xff);
            Mem_CopyHalfwords_Arm7(entry->data, dst, entry->size);
            dst = (u16 *)((u8 *)dst + (entry->size & ~1));
            size += entry->size;
            space -= entry->size;

            if (withSeqNo == TRUE) {
                int seqNo;

                /* bit 0 set: no number given yet */
                if ((entry->seqNo & 1) == 0) {
                    seqNo = entry->seqNo >> 1;
                } else {
                    seqNo = status->portSeqNo[status->aid][entry->port & 7]++;
                    entry->seqNo = seqNo << 1;
                }
                *dst++ = seqNo & 0x7fff;
                size += 2;
                space -= 2;
            }
            if (withDest == TRUE) {
                *header |= 0x1000;
                *dst++ = targets;
                size += 2;
                space -= 2;
            }
            if (first == TRUE) {
                first = FALSE;
            }

            if (ready->tail == index) {
                ready->tail = prev;
            }
            *prevNext = entry->next;
            entry->next = WIRELESS_OUTBOX_NO_ENTRY;
            if (send->tail == WIRELESS_OUTBOX_NO_ENTRY) {
                send->head = index;
            } else {
                queueData[send->tail].next = index;
            }
            send->tail = index;
            entry->unk_0a = targets; /* the stations this frame goes to */
            skippedPorts &= ~portBit;
            destBitmap |= entry->destBitmap;
            index = prev;
            if (status->unk_94 == 1) {
                goto done;
            }

        next:
            if (index != WIRELESS_OUTBOX_NO_ENTRY) {
                prevNext = &queueData[index].next;
            } else {
                prevNext = &ready->head;
            }
            prev = index;
            index = (index != WIRELESS_OUTBOX_NO_ENTRY) ? queueData[index].next : ready->head;
        }
    }

done:
    *destBitmapOut = destBitmap;
    *sizeOut = size;
    Mutex_Release_Arm7(&status->sendQueueMutex);
    return 0;
}
