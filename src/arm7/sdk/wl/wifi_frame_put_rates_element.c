#include "sdk/wl.h"

void WL_WriteByte(void *addr, u32 value);

/* 802.11 rates in 500 kbit/s units (1 to 54 Mbit/s), indexed by the bits of
   WlLink.supportedRates and basicRates. */
extern u16 gWifiRateHalfMbps[16];

// Writes the Supported Rates element (id 1) with one byte per supported rate,
// bit 7 set for a basic rate, and returns its size.
u32 WifiFrame_PutRatesElement(u8 *dest)
{
    WlLink *link = WIFI_LINK_VIEW_OF(WIFI_DRIVER_STATE);
    u32 length = 0;
    u32 i;

    WL_WriteByte(dest, 1);
    length += 2;
    for (i = 0; i < 16; i++) {
        if (link->supportedRates & (1 << i)) {
            if (link->basicRates & (1 << i)) {
                WL_WriteByte(dest + length, (gWifiRateHalfMbps[i] | 0x80) & 0xff);
            } else {
                WL_WriteByte(dest + length, gWifiRateHalfMbps[i] & 0xff);
            }
            length++;
        }
    }
    WL_WriteByte(dest + 1, (length - 2) & 0xff);
    return length;
}
