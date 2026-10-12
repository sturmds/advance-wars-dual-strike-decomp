#include "sdk/wl.h"

u16 WifiBaseband_Get(u32 index)
{
    IO_WIRELESS_BASEBAND_SELECT = index | 0x6000;
    WifiBaseband_AwaitReady();
    return IO_WIRELESS_BASEBAND_IN;
}
