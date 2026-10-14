#include "sdk/wl.h"

void WifiFlash_ReadRadioBytes(u32 offset, u32 size, void* dest);

/* Firmware 0x64-0xcc holds the start values of the 105 baseband registers. */
void WifiBaseband_LoadStartValues(void) {
  u32 i;
  u32 offset;
  u32 value;

  WIFI_MAC_REGISTER(0x160) = 0x100;
  /* Needed to match: the three set in this order, outside the for. */
  offset = 0x64;
  i = 0;
  value = 0;
  for (; i < WIFI_BASEBAND_START_VALUES; offset++, i++) {
    WifiFlash_ReadRadioBytes(offset, 1, &value);
    WifiBaseband_Put(i, value);
  }
}
