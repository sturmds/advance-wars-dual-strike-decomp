#include "sdk/wl.h"

u16 WifiStation_ChallengeSeedOf(u32 station) {
  return WifiStation_Entry(station)->challengeSeed;
}
