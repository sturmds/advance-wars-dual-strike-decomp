#include "sdk/wl.h"

int func_arm7_027c933c(u32 value);

// Wifi register 0x08c is the beacon interval (GBATEK).
int WifiParam_ChangeBeaconPeriod(u16 interval)
{
    if (interval < 10 || interval > 1000) {
        return 5;
    }
    WIFI_DRIVER_STATE->state.unk_86 = interval;
    WIFI_MAC_REGISTER(0x08c) = interval;
    // unk_0c is scaled by the interval; recompute it.
    func_arm7_027c933c(WIFI_DRIVER_STATE->state.unk_0c);
    return 0;
}
