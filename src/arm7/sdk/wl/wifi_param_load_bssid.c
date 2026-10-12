#include "sdk/wl.h"

void func_arm7_027c90ec(u16 *dest, const u16 *src);

int WifiParam_LoadBssid(const u16 *bssid)
{
    func_arm7_027c90ec(WIFI_DRIVER_STATE->state.unk_7c, bssid);
    func_arm7_027c90ec(WIFI_BSSID_REGISTERS, bssid);
    // Bit 0 of the first byte marks a group address.
    if (bssid[0] & 1) {
        WIFI_MAC_REGISTER(0x0d0) &= ~0x400;
    } else {
        WIFI_MAC_REGISTER(0x0d0) |= 0x400;
    }
    return 0;
}
