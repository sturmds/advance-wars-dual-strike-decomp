#include "sdk/wl.h"

void func_arm7_027c9560(void);

int WifiParam_SetShortPreamble(u32 enable)
{
    WlWork *work = WIFI_DRIVER_STATE;
    WlLink *link = WIFI_LINK_VIEW_OF(work);

    if (enable > 1) {
        return 5;
    }
    work->state.flags.raw = (work->state.flags.raw & ~4) | ((enable & 1) << 2);
    if (enable == 0) {
        link->flags_7c &= ~0x20;
    } else {
        link->flags_7c |= 0x20;
    }
    if (link->mode == 64) {
        if (WIFI_DRIVER_STATE->state.phase == 1) {
            WIFI_DRIVER_STATE->state.txFrame->unk_2e = link->flags_7c;
        }
    }
    if (enable == 0) {
        WIFI_MAC_PREAMBLE_BITS &= ~6;
    } else {
        WIFI_MAC_PREAMBLE_BITS |= 6;
    }
    func_arm7_027c9560();
    return 0;
}
