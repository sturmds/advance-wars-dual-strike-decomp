#include "sdk/wl.h"

/* Stations below this state can be replaced by a new one. */
#define WIFI_PEER_REPLACEABLE_BELOW 0x30

void WifiStation_SetUpEntry(u32 station, const u16 *macAddress);

/*
 * Returns the index of the station with this MAC address, setting up a new
 * entry if none has it: the first free entry, or else the idle station
 * (no queued frames) with the lowest unk_18. Returns 0 for a group address
 * and WIFI_NO_PEER when no entry can be taken.
 */
u32 WifiStation_LookUpOrCreate(const u16 *macAddress)
{
    WlStationTable *table;
    u32 lowest;
    u32 count;
    WlStation *station;
    u32 index;
    u32 seen;
    u32 freeIndex;
    WlStation *stations;
    u32 other;

    /* Needed to match: the original keeps one pointer to these fields. */
    table = WIFI_PEER_TABLE_INFO;
    if (macAddress[0] & 1) {
        return 0;
    }
    if (WIFI_DRIVER_STATE->stationBits.stationCount > 1) {
        station = &table->stations[1];
        freeIndex = 0;
        seen = 0;
        for (index = 1; index < table->count; index++, station++) {
            if (station->state != 0) {
                if (WifiMac_Equal(station->macAddress, macAddress)) {
                    return index;
                }
                seen++;
                if (seen >= WIFI_DRIVER_STATE->stationBits.stationCount) {
                    break;
                }
            } else if (freeIndex == 0) {
                freeIndex = index;
            }
        }
        if (freeIndex != 0) {
            index = freeIndex;
        }
    } else {
        index = 1;
    }
    if (index >= WIFI_DRIVER_STATE->limits.unk_f6) {
        stations = table->stations;
        lowest = 0x10000;
        other = 1;
        index = 0;
        for (count = table->count; other < count; other++) {
            if (stations[other].state < WIFI_PEER_REPLACEABLE_BELOW && stations[other].queuedFrames == 0
                && lowest > stations[other].unk_18) {
                lowest = stations[other].unk_18;
                index = other;
            }
        }
        if (index == 0) {
            return WIFI_NO_PEER;
        }
    }
    WifiStation_SetUpEntry(index, macAddress);
    return index;
}
