#include "sdk/wl.h"

int WifiParam_SetBeaconForwarding(int bit6)
{
    if ((u32)bit6 > 1) {
        return 5;
    }
    WIFI_DRIVER_STATE->state.flags.bit6 = (u16)bit6;
    return 0;
}
