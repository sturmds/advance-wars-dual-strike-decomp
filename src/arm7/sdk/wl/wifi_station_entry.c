#include "sdk/wl.h"

WlStation *WifiStation_Entry(u16 index)
{
    // WlLimits.unk_f0 is declared u32; it holds the station table pointer.
    return &((WlStation *)WIFI_DRIVER_STATE->limits.unk_f0)[index];
}
