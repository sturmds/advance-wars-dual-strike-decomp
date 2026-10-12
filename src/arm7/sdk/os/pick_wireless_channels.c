#include "sdk/os_arm7.h"

/*
 * Reduces the firmware's mask of usable wireless channels (bit 0 for
 * channel 1, 13 channels) to at most three: the lowest and the highest, and
 * one near the middle when it is at least 5 channels from both. With less
 * than 5 channels between the lowest and the highest, only the lowest is
 * kept.
 */
u16 PickWirelessChannels(u16 value)
{
    int high;
    int low;
    int mid;
    int i;
    int channels;
    int span;

    channels = (u16)(value & 0x1fff);   /* needed to match: int with the u16 cast, not a u16 local */
    if (channels == 0) {
        return 0;
    }
    for (low = 0; low < 16; low++) {
        if (channels & (1 << low)) {
            break;
        }
    }
    for (high = 15; high != 0; high--) {
        if (channels & (1 << high)) {
            break;
        }
    }
    span = high - low;
    if (span < 5) {
        return 1 << low;
    }
    /* Look at mid, mid + 1, mid - 1, mid + 2, ... for a usable channel. */
    mid = _s32_div_f(high + low, 2);
    for (i = 0; i < span; i++) {
        mid += i * (i % 2 * 2 - 1);
        if (channels & (1 << mid)) {
            break;
        }
    }
    if (high - mid < 5 || mid - low < 5) {
        return (1 << low) | (1 << high);
    }
    return (1 << high) | (1 << mid) | (1 << low);
}
