#include "sdk/wl.h"

u8 WL_ReadByte(const void *addr);

// TRUE when ssid matches the configured SSID, ignoring the bits set in the
// mask. An empty configured SSID matches anything.
BOOL WifiScan_SsidPassesFilter(u32 length, const u8 *ssid)
{
    WlWork *work = WIFI_DRIVER_STATE;
    WlSsidParams *params = WIFI_SSID_SETTINGS_OF(work);
    const u8 *own;
    const u8 *mask;
    u32 i;
    u8 maskByte;
    u8 byte;
    u8 ownByte;

    if (length > 32) {
        return FALSE;
    }
    if (params->ssidLength == 0) {
        return TRUE;
    }
    if (work->state.unk_d8[0] == 19) {
        // A longer ssid matches on its first ssidLength bytes.
        if (length < params->ssidLength) {
            return FALSE;
        }
        length = params->ssidLength;
    } else if (length != params->ssidLength) {
        return FALSE;
    }
    own = params->ssid;
    mask = params->ssidMask;
    for (i = 0; i < length; i++) {
        maskByte = WL_ReadByte(mask);
        mask++;
        byte = WL_ReadByte(ssid);
        ssid++;
        ownByte = WL_ReadByte(own);
        own++;
        if ((byte | maskByte) != (ownByte | maskByte)) {
            return FALSE;
        }
    }
    return TRUE;
}
