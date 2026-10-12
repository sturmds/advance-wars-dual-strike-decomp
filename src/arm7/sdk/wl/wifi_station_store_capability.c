#include "sdk/wl.h"

void WifiStation_StoreCapability(u32 station, u32 capability) {
  WifiStation_Entry(station)->capability = capability;
}
