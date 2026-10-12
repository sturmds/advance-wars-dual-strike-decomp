#include "sdk/wl.h"

void func_arm7_027c92e8(void)
{
    WIFI_DRIVER_STATE->state.unk_be = 1;
    if (WIFI_DRIVER_STATE->state.unk_13c == 0) {
        WIFI_MAC_WAKE_CONTROL &= ~2;
        WIFI_MAC_POWER_AUX = 0;
    }
}
