#include "sdk/wl.h"

void func_arm7_027c92b8(void) {
  WIFI_DRIVER_STATE->state.unk_be = 0;
  WIFI_MAC_WAKE_CONTROL |= 2;
}
