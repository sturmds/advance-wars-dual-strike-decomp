#include "sdk/wl.h"

u32 WifiStation_IndexOfMac(const u16 *macAddress);
/* Needed to match: the station index is passed without narrowing to u16. */
void func_arm7_027c90ec(u16 *dest, const u16 *src);
void WifiTx_EnqueueGroupcast(WlBufferList *from, WlBuffer *buffer);
void WifiStation_CountQueuedFrame(WlTxFrame *frame);
void WifiTx_StartNextGroupcast(void);
void WifiTx_StartNextOfQueue(int arg);

// Fills in the station, rate, frame control and addresses of a data frame from
// the host and queues it: on the parent (phase 1) to a station or, for station
// 0, to the multicast queue; on a child to the parent.
int WifiTx_EnqueueHostData(WlTxBuffer *buffer)
{
    WlWork *work = WIFI_DRIVER_STATE;
    WlLink *link = WIFI_LINK_VIEW_OF(work);
    WlStateView *view = WIFI_FROM_STATION_TABLE_OF(work);
    WlBufferLists *lists = WIFI_LISTS_VIEW_OF(work); /* needed to match */
    WlTxFrame *frame = &buffer->frame;
    u32 station;

    if (buffer->frame.bodyLength > 0x5e4) { /* needed to match: read through buffer, not frame */
        return 5;
    }
    if (view->state.phase == 1) {
        station = WifiStation_IndexOfMac(frame->addr1);
        if (station == WIFI_NO_PEER || WifiStation_StateOf(station) != WIFI_PEER_ASSOCIATED) {
            return 10;
        }
    } else {
        station = link->unk_88;
    }
    frame->station = station;
    frame->unk_04 = WIFI_DRIVER_STATE->state.periodicCount;
    if (frame->requestedRate & 0xff) {
        frame->rate = frame->requestedRate;
        frame->requestedRate = 0;
    } else {
        frame->rate = WifiStation_SendRateOf(station);
    }
    if (frame->bodyLength == 0) {
        frame->frameControl = (link->flags_8a | 0x40) & ~0x4000; /* null function, no WEP */
        frame->length = 28;
    } else {
        frame->frameControl = link->flags_8a;
        // 8 more bytes when unk_08 is set: likely the WEP IV and ICV
        if (view->state.unk_08 == 0) {
            frame->length = frame->bodyLength + 28;
        } else {
            frame->length = frame->bodyLength + 36;
        }
    }
    switch (view->state.phase) {
    case 1:
        func_arm7_027c90ec(frame->addr3, frame->addr2);
        func_arm7_027c90ec(frame->addr2, link->bssid);
        if (station == 0) {
            WifiTx_EnqueueGroupcast(&lists->inbox, &buffer->header);
            if ((WIFI_DRIVER_STATE->stationBits.unk_02 & ~WIFI_DRIVER_STATE->stationBits.unk_06) == 0) {
                WifiTx_StartNextGroupcast();
            }
        } else {
            WifiStation_CountQueuedFrame(frame);
            WifiBuffer_Transfer(&lists->inbox, &lists->txQueues[0], &buffer->header);
            WifiTx_StartNextOfQueue(0);
        }
        break;
    case 2:
    case 3:
        func_arm7_027c90ec(frame->addr3, frame->addr1);
        func_arm7_027c90ec(frame->addr1, link->bssid);
        WifiStation_CountQueuedFrame(frame);
        WifiBuffer_Transfer(&lists->inbox, &lists->txQueues[0], &buffer->header);
        WifiTx_StartNextOfQueue(0);
        break;
    }
    return 0x81;
}
