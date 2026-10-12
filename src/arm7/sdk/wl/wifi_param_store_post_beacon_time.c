#include "sdk/wl.h"

void func_arm7_027c843c(u8 *address, int value);

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
            func_arm7_027c843c(slot, value & 0xff);
            func_arm7_027c843c(slot + 1, (value >> 8) & 0xff);
        } else {
            func_arm7_027c843c(slot, 0xff);
            func_arm7_027c843c(slot + 1, 0xff);
        }
    }
    return 0;
}
