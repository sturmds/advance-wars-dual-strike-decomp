#include "sdk/wl.h"

void WifiStation_StoreLastSeq(u32 station, u32 lastSequence) {
  WifiStation_Entry(station)->lastSequence = lastSequence;
}
