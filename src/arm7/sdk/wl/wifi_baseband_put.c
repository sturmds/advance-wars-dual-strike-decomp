#include "sdk/wl.h"

u32 WifiBaseband_Put(u32 index, u32 value) {
  IO_WIRELESS_BASEBAND_OUT = value;
  IO_WIRELESS_BASEBAND_SELECT = index | 0x5000;
  // Needed to match: an int -1 here compiles to a negate (rsb).
  return WifiBaseband_AwaitReady() ? 0xFFFFFFFF : 0;
}
