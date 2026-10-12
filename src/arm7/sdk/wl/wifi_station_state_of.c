#include "sdk/wl.h"

u16 WifiStation_StateOf(u32 station) {
  return WifiStation_Entry(station)->state;
}
