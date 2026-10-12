#include "sdk/mi.h"
#include "sdk/wl.h"

/*
 * Clears every station entry but entry 0 and resets the station bitmasks so
 * that only station 0 is counted, with AID 0 in use.
 */
void WifiStation_ForgetPeers(void) {
  volatile u16 zero;
  WlStation* stations = WIFI_PEER_ARRAY;
  WlStationBits* bits = &WIFI_DRIVER_STATE->stationBits;
  u32 index;
  u32 count = WIFI_PEER_TABLE_INFO->count;

  for (index = 1; index < count; index++) {
    zero = 0;
    Mem_FillHalfwords_Arm7(zero, &stations[index],
                           sizeof(WlStation) - 2); /* all but unk_1a */
  }
  bits->stationCount = 1;
  bits->unk_02 = 0;
  bits->unk_04 = 1;
  bits->unk_06 = 0xfffe;
  bits->aidCount = 0;
  bits->aidMask = 1;
}
