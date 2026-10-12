#include "sdk/wl.h"

void WL_WriteByte(u8 *address, int value);

int WifiParam_StorePostBeaconTime(u16 value, int writeReg, int unused)
{
    u8 *slot;

    if (value < 10) {
        return 5;
    }
    WIFI_DRIVER_STATE->state.unk_10 = value;
    if (writeReg != 0) {
        WIFI_POST_BEACON_TIMER = value;
    }
    if (WIFI_DRIVER_STATE->state.unk_178 != 0) {
        slot = ((WlPayloadEntry *)(WIFI_DRIVER_STATE->state.txFrame->payload + WIFI_DRIVER_STATE->state.unk_ae))->bytes;
        if (WIFI_DRIVER_STATE->state.unk_26 == 1) {
            WL_WriteByte(slot, value & 0xff);
            WL_WriteByte(slot + 1, (value >> 8) & 0xff);
        } else {
            WL_WriteByte(slot, 0xff);
            WL_WriteByte(slot + 1, 0xff);
        }
    }
    return 0;
}
