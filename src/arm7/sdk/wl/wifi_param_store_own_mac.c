#include "sdk/wl.h"

void func_arm7_027c90ec(u16 *dest, const u16 *src);

int WifiParam_StoreOwnMac(const u16 *macAddress)
{
    // A MAC address with bit 0 of its first byte set is a group address.
    if (macAddress[0] & 1) {
        return 5;
    }
    func_arm7_027c90ec(WIFI_DRIVER_STATE->macAddress, macAddress);
    func_arm7_027c90ec(WIFI_OWN_MAC_REGISTERS, macAddress);
    WIFI_DRIVER_STATE->state.unk_14 |= 2;
    return 0;
}
