#include "sdk/wl.h"

void WifiTx_PlaceFrameInRam(WlRamFrame *dest, WlTxBuffer *buffer);
void WifiStation_UncountQueuedFrame(WlTxFrame *frame);
void WifiHost_ReturnSentFrame(WlBufferList *list, WlTxRequest *request);
int func_arm7_027c94c0(int value);
/* Returns WifiStation_Entry(station)->queuedFrames; its definition takes no argument yet. */
u16 func_arm7_027cb04c(u16 station);
int func_arm7_027cb120(int station);
/* Returns WifiStation_Entry(station)->state; its definition takes no argument yet. */
u16 func_arm7_027cb140(u16 station);
/* Returns buffer->next; its definition still says int. */
int func_arm7_037f8a5c(WlBuffer *buffer);

// Sends the next frame of TX queue `queue` through the TX slot of the same
// number, unless the slot is busy. On the way it drops frames that waited
// longer than WlLink.unk_8c and, for queue 0 and for queue 1 frames to a
// station in state 0x40, skips frames whose station bit in
// stationBits.unk_04 is clear.
void WifiTx_StartNextOfQueue(int queue)
{
    /* needed to match: this declaration order and int frameControl */
    WlLink *link;
    WlTxBuffer *buffer;
    WlMpReplyView *mp;
    WlRamFrame *dest;
    WlTxSlot *slot;
    WlTxFrame *frame;
    volatile u16 *reg;
    u16 station;
    WlBuffer *next;
    WlBufferList *list;
    int frameControl;
    u32 irqMask;
    u32 location;
    WlWork *work;

    work = WIFI_DRIVER_STATE;
    link = WIFI_LINK_VIEW_OF(work);
    mp = WIFI_REPLY_SLOTS_VIEW_OF(work);
    slot = &WIFI_TX_LOCATIONS_OF(work)->slot[queue];
    list = WIFI_SEND_QUEUE_OF(work, queue);

    if (list->count == 0) {
        return;
    }
    irqMask = Irq_ClearEnableBits_Arm7(IRQ_MASK_WIRELESS);
    if (slot->busy) {
        Irq_SetEnableBits_Arm7(irqMask);
        return;
    }
    next = list->first;
    while (1) {
        if (next == WIFI_LIST_END) {
            Irq_SetEnableBits_Arm7(irqMask);
            return;
        }
        buffer = (WlTxBuffer *)next;
        next = (WlBuffer *)func_arm7_037f8a5c(next);
        frame = &buffer->frame;
        station = frame->station;
        if ((u16)(link->periodicCount - frame->unk_04) > link->unk_8c) {
            mp->txTimeouts++;
            frame->result = WIFI_SEND_RESULT_UNSENT;
            slot->sentCount++;
            slot->onDone(frame, FALSE);
            continue;
        }
        if (queue == 0 || (queue == 1 && func_arm7_027cb140(station) == WIFI_PEER_ASSOCIATED)) {
            if (!func_arm7_027cb120(station)) {
                continue;
            }
            if (func_arm7_027cb140(station) != WIFI_PEER_ASSOCIATED) {
                frame->result = WIFI_SEND_RESULT_UNSENT;
                /* needed to match: the request address is taken from frame */
                WifiHost_ReturnSentFrame(list, WIFI_HOST_TX_REQUEST_OF(frame));
                WifiStation_UncountQueuedFrame(frame);
                continue;
            }
        }
        break;
    }
    slot->busy = 1;
    slot->count++;
    slot->queued = frame;
    dest = slot->frame;
    if (link->unk_10 == 0) {
        func_arm7_027c94c0(2);
    }
    WifiTx_PlaceFrameInRam(dest, buffer);
    if (link->unk_0c == 1 && func_arm7_027cb04c(station) > 1) {
        dest->frameControl |= WIFI_FRAMECTL_MORE_BUFFERED;
    }
    frameControl = frame->frameControl;
    reg = REG_W_TXBUF_LOC(queue);
    location = ((u32)dest & 0x3fff) >> 1;
    if ((frameControl & 0xc) == 4) { /* control frame */
        *reg = (u16)location | 0xa000;
    } else if ((frameControl & 0xfc) == WIFI_FRAMECTL_PROBE_ANSWER) {
        *reg = (u16)location | 0x9000;
    } else {
        *reg = (u16)location | W_TXBUF_ENABLE;
    }
    Irq_SetEnableBits_Arm7(irqMask);
}
