#include "sdk/mi.h"
#include "sdk/wl.h"

u16 gWlBroadcastAddress[] = {
#include "data/arm7/gWlBroadcastAddress.inc.c"
};

void WifiStation_SetUpEntry(u32 index, const u16* macAddress);
void WifiStation_ChangeState(u16 station, u16 state);

/*
 * Clears the whole station table and the station bitmasks, sets timeoutReload of
 * every entry to 0xffff, and sets up entry 0 for the broadcast address.
 */
void WifiStation_ClearTable(void) {
  /* Needed to match: one zero variable per clear. */
  volatile u16 zeroTable;
  volatile u16 zeroBits;
  WlStation* stations = WIFI_PEER_ARRAY;
  u32 count = WIFI_PEER_TABLE_INFO->count;
  u32 index;

  zeroTable = 0;
  Mem_FillHalfwords_Arm7(zeroTable, stations, count * sizeof(WlStation));
  zeroBits = 0;
  Mem_FillHalfwords_Arm7(zeroBits, &WIFI_DRIVER_STATE->stationBits,
                         sizeof(WlStationBits));
  stations[0].timeoutReload = 0xffff;
  for (index = 1; index < count; index++) {
    stations[index].timeoutReload = 0xffff;
  }
  WifiStation_SetUpEntry(0, gWlBroadcastAddress);
  WifiStation_ChangeState(0, WIFI_PEER_ASSOCIATED);
}
