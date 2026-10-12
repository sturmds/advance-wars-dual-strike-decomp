#include "sdk/wl.h"

u8 WL_ReadByte(const void *addr);

extern u16 gWlRateBits[120];

// element is an 802.11 Supported Rates element: id, length, then one byte
// per rate in 500 kbit/s units, bit 7 set for a basic rate. A rate the
// table has no bit for sets bit 15.
void WifiRates_DecodeElement(const u8 *element, WlRateSet *rates)
{
    u32 i;
    u32 count;
    u32 rate;
    u32 index;
    u16 bit;

    rates->basic = 0;
    rates->supported = 0;
    count = WL_ReadByte(element + 1);
    for (i = 0; i < count; i++) {
        rate = WL_ReadByte(element + 2 + i);
        index = (rate & 0x7f) - 1;
        if (index < 120 && (bit = gWlRateBits[index]) != 0xff) {
            rates->supported |= 1 << bit;
            if (rate & 0x80) {
                rates->basic |= 1 << bit;
            }
        } else {
            rates->supported |= 0x8000;
            if (rate & 0x80) {
                rates->basic |= 0x8000;
            }
        }
    }
}
