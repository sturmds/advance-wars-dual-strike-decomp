#include "sdk/wl.h"

int func_arm7_027c9a20(u32 value) {
  if (value < 1 || value > 255) {
    return 5;
  }
  WIFI_DRIVER_STATE->state.unk_88 = value;
  return 0;
}
