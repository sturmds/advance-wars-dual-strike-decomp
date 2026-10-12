#include "sdk/wl.h"

void func_arm7_037fde40(const void *src, void *dest, int size);

/*
 * Copies size bytes (rounded up to even) out of the MAC RAM ring buffer.
 * A read that runs past WIFI_RX_RING_LIMIT goes on from the start of the ring.
 */
void WifiRx_CopyFromRingWrapped(void *dest, const void *src, u32 size)
{
    u32 first;
    u32 rest;

    size = (size + 1) & ~1;
    if ((u32)src + size > WIFI_RX_RING_LIMIT) {
        first = WIFI_RX_RING_LIMIT - (u32)src;
        rest = size - first;
    } else {
        first = size;
        rest = 0;
    }
    func_arm7_037fde40(src, dest, first);
    if (rest != 0) {
        func_arm7_037fde40((u8 *)src + first - WIFI_DRIVER_STATE->state.ringSize, (u8 *)dest + first, rest);
    }
}
