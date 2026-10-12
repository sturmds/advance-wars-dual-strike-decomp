#include "sdk/wl.h"

void WifiStation_CountQueuedFrame(WlTxFrame *frame);
void func_arm7_027d0924(WlTxFrame *frame, int arg);
void WifiHost_ReturnSentFrame(WlBufferList *list, WlTxRequest *request);
void func_arm7_027d10f4(int queue);
void func_arm7_027caedc(u16 station);

/*
 * onDone callback of the LOC3 (multicast) slot. A management frame moves on
 * to TX queue 1; any other frame goes back to the host. Without the more-data
 * bit, LOC3 stops and LOC1 and LOC2 start again. TIM bit 0 is cleared once
 * the multicast queue is empty.
 */
void WifiTx_GroupSlotDone(WlTxFrame *frame, int arg)
{
    WlWork *work = WIFI_DRIVER_STATE;
    WlBufferPool *pool = (WlBufferPool *)&work->allocator;
    WlBufferList *multicast = &pool->lists[WIFI_SEND_QUEUE_GROUP];

    work->counters[WIFI_STAT_GROUP_SENT]++;
    if (WIFI_FRAMECTL_FIELDS(frame->frameControl)->type == 0) {
        WifiStation_CountQueuedFrame(frame);
        WifiBuffer_Transfer(multicast, &pool->lists[1], &WIFI_HOST_TX_REQUEST_OF(frame)->header);
        func_arm7_027d0924(frame, 0);
    } else {
        WifiHost_ReturnSentFrame(multicast, WIFI_HOST_TX_REQUEST_OF(frame));
    }
    WIFI_TX_LOCATIONS_OF(WIFI_DRIVER_STATE)->slot[WIFI_TXLOC_GROUP_OR_PSPOLL].busy = 0;
    if (!WIFI_FRAMECTL_FIELDS(WIFI_TX_LOCATIONS_OF(WIFI_DRIVER_STATE)->slot[WIFI_TXLOC_GROUP_OR_PSPOLL].frame->frameControl)->moreData) {
        REG_W_TXREQ_RESET = W_TXREQ_LOC3;
        REG_W_TXREQ_SET = W_TXREQ_LOC1 | W_TXREQ_LOC2;
        if (arg) {
            if (pool->lists[1].count) {
                func_arm7_027d10f4(1);
            }
            if (pool->lists[0].count) {
                func_arm7_027d10f4(0);
            }
        }
    }
    if (multicast->count) {
        if (arg) {
            func_arm7_027d10f4(WIFI_SEND_QUEUE_GROUP);
        }
    } else {
        func_arm7_027caedc(0);
    }
}
