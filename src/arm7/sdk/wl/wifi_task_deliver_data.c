#include "sdk/wl.h"

void func_arm7_027cb3c8(u16 station, u16 rssi);

u32 WifiStation_IndexOfMac(const u16 *macAddress);
void func_arm7_027cb364(u16 station, u32 set);
void func_arm7_027c90ec(u16 *dest, const u16 *src); /* copies a 6-byte MAC address */
int func_arm7_027c94c0(int value);
WlTxFrame *WifiFrame_NewDisassociation(const u16 *dest, u32 reason);
WlTxFrame *WifiFrame_NewDeauthentication(const u16 *dest, u32 reason);
void WifiTx_EnqueueMgmt(void *frame);

/*
 * Task 6: takes the next received data frame from its list and, while
 * associated, passes it to the host as indication 0x180. As a parent
 * (WlLink.unk_0c 1) the sender must be an associated station; others get a
 * disassociation or deauthentication (reason 7). As a child in power save
 * the pending bits of WlLink.unk_8e clear once a frame comes without More
 * Data. A repeated sequence number is counted and the frame freed. Queues itself
 * again while frames are left.
 */
void WifiTask_DeliverData(void)
{
    /* needed to match: buffer and frame declared first */
    WlRxBuffer *buffer;
    WlRxFrame *frame;
    WlWork *work = WIFI_DRIVER_STATE;
    WlLink *link = WIFI_LINK_VIEW_OF(work);
    WlBufferPool *pool = (WlBufferPool *)&work->allocator;
    u32 *counters = work->counters;
    u32 station;
    BOOL drop;
    u16 phase;

    buffer = (WlRxBuffer *)pool->lists[4].first;
    if (&buffer->header == WIFI_LIST_END) {
        return;
    }
    if (link->mode != WIFI_MODE_JOINED) {
        WifiBuffer_Release(&pool->lists[4], &buffer->header);
        return;
    }
    frame = &buffer->frame;
    /* counters 10 and 11: unicast and group-addressed frames */
    if (WIFI_FRAMECTL_FIELDS(frame->frameControl)->toDs) {
        if (frame->addr3[0] & 1) {
            counters[11]++;
        } else {
            counters[10]++;
        }
    } else {
        if (frame->addr1[0] & 1) {
            counters[11]++;
        } else {
            counters[10]++;
        }
    }
    /* bits 4-7 of the RX header word, less one */
    counters[9] += _s32_div_f(frame->unk_08 & 0xf0, 16) - 1;
    drop = TRUE;
    phase = link->unk_0c;
    switch (phase) {
    case 1:
        if (frame->frameControl & 1) {
            break;
        }
        station = WifiStation_IndexOfMac(frame->addr2);
        if (station == WIFI_NO_PEER || WifiStation_StateOf(station) != WIFI_PEER_ASSOCIATED) {
            WlTxFrame *reply;

            if (WifiStation_StateOf(station) == WIFI_PEER_AUTH_DONE) {
                reply = WifiFrame_NewDisassociation(frame->addr2, 7);
            } else {
                reply = WifiFrame_NewDeauthentication(frame->addr2, 7);
            }
            if (reply != NULL) {
                WifiTx_EnqueueMgmt(reply);
            }
            break;
        }
        func_arm7_027cb364(station, WIFI_FRAMECTL_FIELDS(frame->frameControl)->powerManagement);
        if (frame->sequence == WifiStation_LastSeqOf(station)) {
            counters[15]++;
            break;
        }
        /* a To DS frame: its destination is addr3 */
        func_arm7_027c90ec(frame->addr1, frame->addr3);
        drop = FALSE;
        break;
    case 2:
    case 3:
        if (frame->frameControl & 1) {
            break;
        }
        if (link->unk_0e != 0 && !(frame->frameControl & WIFI_FRAMECTL_MORE_BUFFERED)) {
            if (frame->addr1[0] & 1) {
                link->unk_8e &= ~1;
            } else if (phase != 3) {
                link->unk_8e &= ~2;
            }
            if (link->unk_8e == 0 && pool->lists[0].count == 0 && pool->lists[1].count == 0) {
                func_arm7_027c94c0(1);
            }
        }
        station = link->unk_88;
        if (frame->sequence == WifiStation_LastSeqOf(station)) {
            counters[15]++;
            break;
        }
        /* a From DS frame: its source is addr3 */
        func_arm7_027c90ec(frame->addr2, frame->addr3);
        drop = FALSE;
        break;
    }
    if (!drop) {
        frame->station = station;
        func_arm7_027cb3c8(station, frame->unk_12 & 0xff);
        WifiStation_StoreLastSeq(station, frame->sequence);
        func_arm7_027cb27c(station);
        frame->bodyLength = frame->length - WIFI_80211_HEADER_BYTES;
        buffer->command = 0x180;
        buffer->length = (frame->bodyLength + 45u) >> 1;
        WifiBuffer_GiveToHost(&pool->lists[4], &buffer->header);
    } else {
        WifiBuffer_Release(&pool->lists[4], &buffer->header);
    }
    if (pool->lists[4].count != 0) {
        WifiTask_Schedule(2, 6);
    }
}
