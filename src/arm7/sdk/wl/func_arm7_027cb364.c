#include "sdk/wl.h"

/*
 * Sets or clears a station's bit in the mask at 0x506. If any station then has
 * its bit at 0x506 set but not at 0x50a, the LOC3 transmit request is
 * cleared; otherwise it is set.
 */
void func_arm7_027cb364(u16 station, u32 set)
{
    WlStationBits *bits = &WIFI_DRIVER_STATE->stationBits;

    bits->unk_02 = (bits->unk_02 & ~(1 << station)) | (set << station);
    if (bits->unk_02 & ~bits->unk_06) {
        REG_W_TXREQ_RESET = W_TXREQ_LOC3;
    } else {
        REG_W_TXREQ_SET = W_TXREQ_LOC3;
    }
}
