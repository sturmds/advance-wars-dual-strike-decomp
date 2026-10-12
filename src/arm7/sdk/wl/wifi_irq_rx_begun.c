#include "sdk/wl.h"

void func_arm7_027cab1c(void);

/* Frame control of an MP poll from the parent (data + CF-Poll, from the DS),
   with the retry and power management bits masked out. */
#define WIFI_FRAMECTL_PARENT_POLL 0x228
#define WIFI_FRAMECTL_IGNORE_RETRY_PM 0xe7ff

/* Halfwords received since the frame started, across the ring's end. */
static inline u16 WifiRx_HalfwordsSoFar(WlLink *link, u16 start)
{
    u16 n = REG_W_RXTX_ADDR - start;

    if (n & 0x8000) {
        n += (u32)link->ringSize >> 1;
    }
    return n;
}

// RX start interrupt. With option bit 5 set and an MP reply
// queued, follows an incoming frame while the hardware writes it: if it is an
// MP poll from our BSSID whose child bitmap leaves out our AID, takes the
// queued reply back so it is not sent.
void WifiIrq_RxBegun(void)
{
    /* needed to match: this declaration order */
    WlWork *work;
    u16 *p;
    WlLink *link;
    u16 start;
    u16 received;
    WlTxSlots *view;
    u32 i;
    s32 t0;

    work = WIFI_DRIVER_STATE;
    link = WIFI_LINK_VIEW_OF(work);
    view = WIFI_TX_LOCATIONS_OF(work);
    WIFI_IRQ_PENDING_FLAGS = W_IRQ_RX_START;
    if (!(WIFI_DRIVER_STATE->unk_638 & 0x20)) {
        return;
    }
    if (view->savedMpReply != 0xffff) {
        return;
    }
    if ((REG_W_RF_PINS & 3) != 3) {
        return;
    }
    /* needed to match: the casts give lsr and a signed compare */
    if (REG_W_RXTX_ADDR < (s32)(((u32)REG_W_RXBUF_BEGIN >> 1) & 0xfff)) {
        return;
    }
    start = REG_W_RXBUF_WRCSR;
    /* skip the 12-byte RX header to the frame control */
    p = WifiRx_FoldRingPointer((u16 *)&WIFI_PACKET_RAM[start] + 4);
    p = WifiRx_FoldRingPointer(p + 2);
    if ((*p & WIFI_FRAMECTL_IGNORE_RETRY_PM) != WIFI_FRAMECTL_PARENT_POLL) {
        return;
    }
    p = WifiRx_FoldRingPointer(p + 1);
    t0 = REG_W_US_COUNT0 - 0x10000; /* needed to match: the 0x10000 */
    /* wait (up to 64 us) until addr2 has arrived */
    while (1) {
        received = WifiRx_HalfwordsSoFar(link, start);
        if (received > 14) {
            break;
        }
        if ((u16)(REG_W_US_COUNT0 - t0) > 64) {
            return;
        }
    }
    p += 4;
    for (i = 0; i < 3; i++) {
        u16 *q = WifiRx_FoldRingPointer(p);
        p = q + 1;
        if (*q != link->bssid[i]) {
            return;
        }
    }
    p = WifiRx_FoldRingPointer(p + 5);
    /* wait (up to 112 us) until the child bitmap has arrived */
    while (1) {
        received = WifiRx_HalfwordsSoFar(link, start);
        if (received > 20) {
            break;
        }
        if ((u16)(REG_W_US_COUNT0 - t0) > 112) {
            return;
        }
    }
    if (*p & view->aidBit) {
        return;
    }
    view->savedMpReply = REG_W_TXBUF_REPLY2;
    REG_W_TXBUF_RESET = W_TXBUF_RESET_REPLY2;
    link->mpRepliesCancelled++;
    while ((REG_W_RF_PINS & 3) == 3) {
    }
    WIFI_MAC_REGISTER(0x244) |= 0x40;
    WIFI_MAC_REGISTER(0x244) &= ~0x40;
    WIFI_MAC_REGISTER(0x228) = 8;
    WIFI_MAC_REGISTER(0x228) = 0;
    func_arm7_027cab1c();
}
