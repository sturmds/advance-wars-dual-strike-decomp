#include "sdk/wl.h"

u32 WifiStation_IndexOfMac(const u16 *macAddress);
void WifiTx_DropStationFrames(u32 station);

/* Takes the queued frames for a MAC address off the TX queues; a group address means every station. */
void WifiTx_DropFramesToMac(const u16 *macAddress)
{
    u32 station;

    if (macAddress[0] & 1) {
        for (station = 1; station < WIFI_PEER_TABLE_INFO->count; station++) {
            WifiTx_DropStationFrames(station);
        }
    } else {
        station = WifiStation_IndexOfMac(macAddress);
        if (station != WIFI_NO_PEER) {
            WifiTx_DropStationFrames(station);
        }
    }
}
