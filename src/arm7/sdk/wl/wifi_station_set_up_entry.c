#include "sdk/wl.h"
#include "sdk/mi.h"

void func_arm7_027cb364(u16 station, u32 set);
/* Needed to match: called without narrowing the index, unlike the u16 calls
   around it, so the original declared it with a 32-bit parameter. */
void func_arm7_027c90ec(u16* dest,
                        const u16* src); /* copies a 6-byte MAC address */
void WifiStation_ChangeState(u16 station, u16 state);

/*
 * Sets up station entry `index` for a new peer: counts it if the entry was
 * free, clears it, stores the MAC address and our supported rates, and puts
 * it in the unauthenticated state.
 */
void WifiStation_SetUpEntry(u32 index, const u16* macAddress) {
  volatile u16 zero;
  WlStation* station = &WIFI_PEER_ARRAY[index];

  if (station->state == 0) {
    WIFI_DRIVER_STATE->stationBits.stationCount++;
  }
  zero = 0;
  Mem_FillHalfwords_Arm7(zero, station,
                         sizeof(WlStation) - 2); /* all but timeoutReload */
  func_arm7_027cb364(index, 0);
  func_arm7_027cb300(index);
  func_arm7_027c90ec(station->macAddress, macAddress);
  station->lastSequence = 0xffff;
  station->rates = WIFI_DRIVER_STATE->state.unk_7a;
  station->timeout = station->timeoutReload;
  WifiStation_ChangeState(index, WIFI_PEER_KNOWN_ONLY);
}
