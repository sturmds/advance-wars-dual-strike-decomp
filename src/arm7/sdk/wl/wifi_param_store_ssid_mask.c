#include "sdk/wl.h"

int WifiParam_StoreSsidMask(const u16* words) {
  u32 i;
  u16* dst = WIFI_DRIVER_STATE->state.words_58;

  for (i = 0; i < 16; i++) {
    *dst++ = *words++;
  }
  return 0;
}
