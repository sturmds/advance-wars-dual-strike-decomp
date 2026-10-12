#include "sdk/wl.h"

void WifiStation_UncountQueuedFrame(WlTxFrame *frame);
void WifiHost_ReturnSentFrame(WlBufferList *list, WlTxRequest *request);
int func_arm7_027cb100(int station);
void func_arm7_027cb324(u16 station);
/* Returns WifiStation_Entry(station)->queuedFrames; its definition takes no argument yet. */
u16 func_arm7_027cb04c(u16 station);
int func_arm7_027c94c0(int value);
void func_arm7_027d10f4(int queue);

// TX done callback of the LOC1 slot, which sends TX queue 0: counts the
// result, answers the host, frees the slot and, when sendNext is set, starts
// the next queued frame.
void WifiTx_DataSlotDone(WlTxFrame *frame, int sendNext)
{
    WlWork *work = WIFI_DRIVER_STATE;
    WlBufferList *queue = &work->txQueues[0];
    WlLink *link = WIFI_LINK_VIEW_OF(work);
    u32 *counters = work->counters;
    WlTxRequest *request = WIFI_HOST_TX_REQUEST_OF(frame);

    WifiStation_UncountQueuedFrame(frame);
    if (!(frame->result & 2)) {
        counters[0]++;
        if (WIFI_FRAMECTL_FIELDS(frame->frameControl)->toDs) {
            if (frame->addr3[0] & 1) {
                counters[WIFI_STAT_GROUP_SENT]++;
            } else {
                counters[4]++;
            }
        } else {
            if (frame->addr1[0] & 1) {
                counters[WIFI_STAT_GROUP_SENT]++;
            } else {
                counters[4]++;
            }
        }
    } else {
        counters[1]++;
    }
    if (WIFI_FRAMECTL_FIELDS(frame->frameControl)->protectedFrame) {
        counters[6]++;
    }
    WifiHost_ReturnSentFrame(queue, request);
    WIFI_TX_LOCATIONS_OF(WIFI_DRIVER_STATE)->slot[WIFI_TXLOC_DATA].busy = 0;

    if (func_arm7_027cb100(frame->station) && !(frame->frameControl & WIFI_FRAMECTL_MORE_BUFFERED)) {
        func_arm7_027cb324(frame->station);
    }
    if (sendNext) {
        if (queue->count) {
            func_arm7_027d10f4(0);
        } else if ((link->unk_0c == 2 || link->unk_0c == 3) && link->mode == WIFI_MODE_JOINED
                   && link->unk_0e && func_arm7_027cb04c(link->unk_88) == 0 && link->unk_8e == 0) {
            func_arm7_027c94c0(1);
        }
    }
}
