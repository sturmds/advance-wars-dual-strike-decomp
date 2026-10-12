#include "sdk/wl.h"

void WifiFlash_ReadRadioBytes(u32 offset, u32 size, void* dest);
int func_arm7_027c8c00(void);

void func_arm7_027c9560(void) {
  u32 value = 0xe2e2; /* kept when the wifi block was never loaded */

  WifiFlash_ReadRadioBytes(0x58, 2, &value);
  value += 0x202;
  if (func_arm7_027c8c00() == 20) {
    value -= 0x6161;
    if (WIFI_MAC_PREAMBLE_BITS & 2) {
      value -= 0x6060;
    }
  }
  WIFI_MAC_REGISTER(0x140) = value;
}
