#include "sdk/wl.h"

void WifiStation_StoreChallengeSeed(u32 station, u32 challengeSeed) {
  WifiStation_Entry(station)->challengeSeed = challengeSeed;
}
