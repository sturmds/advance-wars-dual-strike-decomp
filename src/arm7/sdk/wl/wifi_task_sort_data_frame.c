#include "types.h"
#include "sdk/wl.h"

void func_arm7_027c90ec(u16 *dest, const u16 *src); /* copies a 6-byte MAC address */
void WifiRx_AppendFragment(u16 *rxHeader, WlRxAddrs *addrs);
void WifiRx_BeginReassembly(u16 *rxHeader, WlRxAddrs *addrs);

/*
 * Task 9: takes the next received data frame from its list, works out its
 * destination and source from the To DS and From DS bits and hands it on.
 * Frames with both bits set are ignored. Queues itself again while frames
 * are left.
 */
void WifiTask_SortDataFrame(void)
{
    WlWork *work = WIFI_DRIVER_STATE;
    WlBufferPool *pool = (WlBufferPool *)&work->allocator;
    WlRxBuffer *buffer = (WlRxBuffer *)pool->lists[7].first;
    WlRxFrame *frame;
    WlRxAddrs addrs;
    u16 frameControl;

    if (&buffer->header == WIFI_LIST_END) {
        return;
    }
    frame = &buffer->frame;
    if (work->state.mode == WIFI_MODE_JOINED && frame->length <= WIFI_RX_FRAME_CAP) {
        frameControl = frame->frameControl;
        if (frameControl & WIFI_FRAMECTL_SENT_TO_AP) {
            func_arm7_027c90ec(addrs.dest, frame->addr3);
            if (frameControl & WIFI_FRAMECTL_SENT_BY_AP) {
                goto done;
            }
            func_arm7_027c90ec(addrs.src, frame->addr2);
        } else {
            func_arm7_027c90ec(addrs.dest, frame->addr1);
            if (frameControl & WIFI_FRAMECTL_SENT_BY_AP) {
                func_arm7_027c90ec(addrs.src, frame->addr3);
            } else {
                func_arm7_027c90ec(addrs.src, frame->addr2);
            }
        }
        addrs.sequence = frame->sequence;
        if ((frameControl & WIFI_FRAMECTL_FRAGMENTS_FOLLOW) && frame->fragment == 0) {
            WifiRx_BeginReassembly(&frame->unk_08, &addrs);
        } else {
            WifiRx_AppendFragment(&frame->unk_08, &addrs);
        }
    }
done:
    WifiBuffer_Release(&pool->lists[7], &buffer->header);
    if (pool->lists[7].count != 0) {
        WifiTask_Schedule(2, 9);
    }
}
