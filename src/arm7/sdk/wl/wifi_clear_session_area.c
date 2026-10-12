#include "sdk/wl.h"

void func_arm7_037fde28(u16 data, void *dest, int size);

void Wifi_ClearSessionArea(u32 value, int limit)
{
    // The clear of the link area below wipes unk_b4; keep it.
    void *saved = WIFI_DRIVER_STATE->state.unk_b4;
    volatile u16 zeroHead = 0;
    volatile u16 zeroLink;

    func_arm7_037fde28(zeroHead, &WIFI_DRIVER_STATE->limits.unk_f0, 40);
    zeroLink = 0;
    func_arm7_037fde28(zeroLink, WIFI_LINK_VIEW_OF(WIFI_DRIVER_STATE), 192);
    WIFI_DRIVER_STATE->limits.unk_f0 = value;
    WIFI_DRIVER_STATE->limits.unk_f4 = (u16)limit;
    WIFI_DRIVER_STATE->limits.unk_f6 = (u16)limit;
    WIFI_DRIVER_STATE->state.unk_b4 = saved;
}
