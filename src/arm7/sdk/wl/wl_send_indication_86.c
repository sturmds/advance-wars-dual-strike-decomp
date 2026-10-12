#include "types.h"
#include "sdk/wl.h"

u8 WL_ReadByte(const void* addr);
void WL_WriteByte(void* addr, u8 value);
void func_arm7_027c90ec(void* dest,
                        const void* src); /* copies a 6-byte MAC address */

/*
 * Sends host message 0x86 with a MAC address, the low 12 bits of value and
 * the SSID from an 802.11 SSID element (id, length, bytes). The SSID is cut
 * to 32 bytes and padded with zeros. Returns 0 when no buffer is left.
 */
int WifiHost_TellAssociated(const u8* mac, u16 value, const u8* ssidElement) {
  WlIndication* ind =
      (WlIndication*)WifiBuffer_New(&WIFI_DRIVER_STATE->unk_15c, 58);
  u32 i;

  if (ind == NULL) {
    WifiHost_RaiseEventBits(WIFI_EVT_NOTE_DROPPED);
    return 0;
  }
  ind->command = 0x86;
  ind->length = 21;
  func_arm7_027c90ec(ind->peer.mac, mac);
  ind->peer.value = value & 0xfff;
  ind->peer.ssidLength = WL_ReadByte(ssidElement + 1);
  for (i = 0; i < ind->peer.ssidLength; i++) {
    if (i >= WIFI_SSID_LONGEST) {
      break;
    }
    WL_WriteByte(&ind->peer.ssid[i], WL_ReadByte(ssidElement + 2 + i));
  }
  for (; i < WIFI_SSID_LONGEST; i++) {
    WL_WriteByte(&ind->peer.ssid[i], 0);
  }
  WifiBuffer_GiveToHost(&WIFI_DRIVER_STATE->unk_15c, &ind->buffer);
  return 1;
}
