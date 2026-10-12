#include "sdk/wl.h"

/*
 * Returns the index of the station with this MAC address: 0 for a group
 * address, WIFI_NO_PEER when no station has it.
 */
u32 WifiStation_IndexOfMac(const u16* macAddress) {
  WlStation* station;
  u32 index;
  u32 seen;

  if (macAddress[0] & 1) {
    return 0;
  }
  if (WIFI_DRIVER_STATE->stationBits.stationCount > 1) {
    station = &WIFI_PEER_ARRAY[1];
    seen = 0;
    for (index = 1; index < WIFI_DRIVER_STATE->limits.unk_f6;
         index++, station++) {
      if (station->state != 0) {
        if (WifiMac_Equal(station->macAddress, macAddress)) {
          return index;
        }
        seen++;
        if (seen >= WIFI_DRIVER_STATE->stationBits.stationCount) {
          break;
        }
      }
    }
  }
  return WIFI_NO_PEER;
}
