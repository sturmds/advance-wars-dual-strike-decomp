#include "sdk/wl.h"

WlRxFrame *WifiRx_TakeFromRing(const void *src, u32 length);
void WifiRx_HandleBeaconFrame(WlRxFrame *frame);
int WifiChild_ForwardMpPoll(WlRxFrame *frame);
int WifiChild_ForwardMpAck(WlRxFrame *frame);
void WifiParent_StoreMpReply(WlRxFrame *frame);

/*
 * Task 15: walks the receive ring from the read cursor up to work offset
 * WlRxState.readPos, copies each frame into a buffer and hands it on by the hardware
 * frame type (low 4 bits of WlRxFrame.unk_08): fragments to task 9, data to
 * task 6, management and PS-Poll to task 7, beacons and MP frames to their
 * handlers. With bit 0 of unk_638 set it overwrites the ring header with
 * 0xffff after each frame.
 */
void WifiTask_DrainRxRing(void)
{
    u32 count;
    WlRxState *rx;
    u32 i;
    u16 length;
    WlBufferPool *pool;
    u16 cursor;
    u16 *ramFrame;
    WlRxFrame *frame;
    u16 next;
    int freeIt;
    u32 rxStatus;

    rx =&WIFI_DRIVER_STATE->wepWatch;
    pool = (WlBufferPool *)&WIFI_DRIVER_STATE->allocator;

    for (count = 0;; count++) {
        cursor = REG_W_RXBUF_READCSR;
        if (cursor == rx->readPos) {
            break;
        }
        /* Needed to match: the byte offset written out twice, so mwcc keeps cursor * 2 across the call. */
        ramFrame = (u16 *)((u32)WIFI_PACKET_RAM + cursor * 2);
        next = *WifiRx_FoldRingPointer(ramFrame + 1);
        if (*(vu16 *)((u32)WIFI_PACKET_RAM + cursor * 2) == 0xffff) {
            REG_W_RXBUF_READCSR = next;
            continue;
        }
        length = *WifiRx_FoldRingPointer(ramFrame + 4);
        frame = WifiRx_TakeFromRing(ramFrame, length);
        REG_W_RXBUF_READCSR = next;
        if (frame == NULL) {
            if ((ramFrame[0] & 0xf) == 12) {
                WifiHost_RaiseEventBits(16);
            } else {
                WifiHost_RaiseEventBits(8);
            }
            continue;
        }
        if (WIFI_DRIVER_STATE->unk_638 & 8) {
            if (frame->frameControl & WIFI_FRAMECTL_WEP_ON) {
                rx->busyCount = 0;
            }
        }
        rxStatus = frame->unk_08;
        freeIt = TRUE;
        if (rxStatus & 0x200) {
            if (WIFI_FRAMECTL_FIELDS(frame->frameControl)->moreFragments == 1 || frame->fragment != 0) {
                freeIt = FALSE;
                WifiBuffer_Transfer(&pool->newBuffers, &pool->lists[7], &WIFI_RX_HOLDER_OF(frame)->header);
                WifiTask_Schedule(2, 9);
            }
        } else {
            switch (rxStatus & 0xf) {
            case 8:
                if ((frame->frameControl & 0xf) == 8) {
                    freeIt = FALSE;
                    WifiBuffer_Transfer(&pool->newBuffers, &pool->lists[4], &WIFI_RX_HOLDER_OF(frame)->header);
                    WifiTask_Schedule(2, 6);
                }
                break;
            case 1:
                if (frame->frameControl == 0x80) {
                    WifiRx_HandleBeaconFrame(frame);
                }
                break;
            case 0:
                if ((frame->frameControl & 0xf) == 0) {
                    freeIt = FALSE;
                    WifiBuffer_Transfer(&pool->newBuffers, &pool->lists[6], &WIFI_RX_HOLDER_OF(frame)->header);
                    WifiTask_Schedule(1, 7);
                }
                break;
            case 5:
                if ((frame->frameControl & 0xe7ff) == 0xa4) {
                    freeIt = FALSE;
                    WifiBuffer_Transfer(&pool->newBuffers, &pool->lists[6], &WIFI_RX_HOLDER_OF(frame)->header);
                    WifiTask_Schedule(1, 7);
                }
                break;
            case 14:
            case 15:
                if ((frame->frameControl & 0xe7bf) == 0x118) {
                    WifiParent_StoreMpReply(frame);
                }
                break;
            case 12:
                if ((frame->frameControl & 0xe7ff) == 0x228) {
                    if (WIFI_LINK_VIEW_OF(WIFI_DRIVER_STATE)->unk_10 == 0) {
                        WIFI_MAC_REGISTER(0x03c) = 1;
                    }
                    WIFI_DRIVER_STATE->counters[28]++;
                    freeIt = WifiChild_ForwardMpPoll(frame);
                }
                break;
            case 13:
                if ((frame->frameControl & 0xe7ff) == 0x218) {
                    WIFI_DRIVER_STATE->counters[29]++;
                    freeIt = WifiChild_ForwardMpAck(frame);
                }
                break;
            }
        }
        if (freeIt) {
            WifiBuffer_Release(&pool->newBuffers, &WIFI_RX_HOLDER_OF(frame)->header);
        }
        if (WIFI_DRIVER_STATE->unk_638 & 1) {
            for (i = 0; i < 7; i++) {
                if ((u32)ramFrame >= WIFI_RX_RING_LIMIT) {
                    ramFrame = (u16 *)((u8 *)ramFrame - WIFI_DRIVER_STATE->state.ringSize);
                }
                *ramFrame++ = 0xffff;
            }
        }
    }
}
