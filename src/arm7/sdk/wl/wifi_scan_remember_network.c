#include "sdk/mi.h"
#include "sdk/wl.h"

u8 WL_ReadByte(const void* addr);
void WL_WriteByte(void* addr, u32 value);
void func_arm7_027c90ec(u16* dest,
                        const u16* src); /* copies a 6-byte MAC address */

/*
 * Records the BSS of a received beacon in the BSS list: its own entry if
 * the BSSID is already there, else a free entry, else the one with the
 * least time left. Beacons with an SSID longer than 32 bytes are ignored.
 */
void WifiScan_RememberNetwork(u16 value, WlRxFrame* frame,
                              const u8* ssidElement) {
  WlBssEntry* entry = WIFI_DRIVER_STATE->limits.bssList;
  WlBssEntry* list;
  u32 i;
  u32 freeIndex;
  u32 oldestIndex;
  u16 minTtl;
  volatile u16 zero;

  if (WL_ReadByte(ssidElement + 1) > WIFI_SSID_LONGEST) {
    return;
  }
  freeIndex = WIFI_REMEMBERED_NETWORKS;
  oldestIndex = WIFI_REMEMBERED_NETWORKS;
  minTtl = WIFI_NETWORK_LIFETIME;
  for (i = 0; i < WIFI_REMEMBERED_NETWORKS; i++, entry++) {
    if (entry->rssi != 0) {
      if (WifiMac_Equal(entry->bssid, frame->addr3)) {
        goto found;
      }
      if (entry->ttl < minTtl) {
        minTtl = entry->ttl;
        oldestIndex = i;
      }
    } else {
      freeIndex = i;
    }
  }
  if (freeIndex != WIFI_REMEMBERED_NETWORKS) {
    i = freeIndex;
  } else if (oldestIndex != WIFI_REMEMBERED_NETWORKS) {
    i = oldestIndex;
  } else {
    return;
  }
found:
  /* needed to match: the second pointer to the list */
  list = WIFI_DRIVER_STATE->limits.bssList;
  entry = &list[i];
  zero = 0;
  Mem_FillHalfwords_Arm7(zero, entry, sizeof(WlBssEntry));
  entry->ttl = WIFI_NETWORK_LIFETIME;
  list[i].rssi = frame->unk_12 & 0xff;
  entry->unk_02 = value;
  func_arm7_027c90ec(entry->bssid, frame->addr3);
  entry->ssidLength = WL_ReadByte(ssidElement + 1);
  for (i = 0; i < entry->ssidLength; i++) {
    WL_WriteByte(&entry->ssid[i], WL_ReadByte(ssidElement + 2 + i));
  }
  /* a beacon body has the probe response layout */
  entry->beaconInterval =
      WIFI_MANAGEMENT_FIELDS_OF(frame)->probeResponse.beaconInterval;
  entry->capability =
      WIFI_MANAGEMENT_FIELDS_OF(frame)->probeResponse.capability;
}
