#include "sdk/wl.h"

// Wraps a pointer into the RX ring in WIFI_PACKET_RAM: past the ring's end it
// continues at the start.
u16 *WifiRx_FoldRingPointer(u16 *p)
{
    if ((u32)p >= WIFI_RX_RING_LIMIT) {
        p = (u16 *)((u8 *)p - WIFI_DRIVER_STATE->state.ringSize);
    }
    return p;
}
