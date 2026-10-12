#include "sdk/wl.h"

int WifiBaseband_SetCcaParams(int reg13Value, int reg35Value, void* request) {
  if ((u32)reg13Value > 3) {
    return 5;
  }
  if ((u32)reg35Value > 0x3f) {
    return 5;
  }
  WifiBaseband_Put(0x13, reg13Value);
  WifiBaseband_Put(0x35, reg35Value);
  return 0;
}
