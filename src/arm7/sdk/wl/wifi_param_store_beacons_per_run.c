#include "sdk/wl.h"

int WifiParam_StoreBeaconsPerRun(u32 value)
{
    if (value > 255) {
        return 5;
    }
    WIFI_DRIVER_STATE->state.unk_98 = 0;
    WIFI_DRIVER_STATE->state.unk_96 = value;
    return 0;
}
