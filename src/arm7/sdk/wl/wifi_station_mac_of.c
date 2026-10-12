#include "sdk/wl.h"

u16* WifiStation_MacOf(u32 station) {
  return WifiStation_Entry(station)->macAddress;
}
