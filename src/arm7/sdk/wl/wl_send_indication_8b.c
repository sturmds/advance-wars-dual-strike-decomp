#include "types.h"
#include "sdk/wl.h"

void func_arm7_027c90ec(void* dest,
                        const void* src); /* copies a 6-byte MAC address */

/* Sends host message 0x8b with a MAC address. Returns 0 when no buffer is left. */
int WifiHost_TellBeaconLost(const u8* mac) {
  WlIndication* ind =
      (WlIndication*)WifiBuffer_New(&WIFI_DRIVER_STATE->unk_15c, 22);

  if (ind == NULL) {
    WifiHost_RaiseEventBits(WIFI_EVT_NOTE_DROPPED);
    return 0;
  }
  ind->command = 0x8b;
  ind->length = 3;
  func_arm7_027c90ec(ind->peer.mac, mac);
  WifiBuffer_GiveToHost(&WIFI_DRIVER_STATE->unk_15c, &ind->buffer);
  return 1;
}
