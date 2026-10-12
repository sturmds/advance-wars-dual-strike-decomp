#include "sdk/wl.h"

void func_arm7_027c9438(void)
{
    u32 value;

    if (WIFI_DRIVER_STATE->rf.type == 2) {
        WifiRf_SendWord(0xc008);
    }
    value = WifiBaseband_Get(30) | 0x3f;
    WifiBaseband_Put(30, value);
    WIFI_MAC_REGISTER(0x168) = 0x800d;
    WIFI_MAC_REGISTER(0x036) = 1;
}
