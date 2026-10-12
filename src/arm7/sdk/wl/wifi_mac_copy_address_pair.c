#include "sdk/wl.h"

// Copies two 6-byte MAC addresses into consecutive slots of dest.
void WifiMac_CopyAddressPair(u16* dest, const u16* addr1, const u16* addr2) {
  dest[0] = addr1[0];
  dest[1] = addr1[1];
  dest[2] = addr1[2];
  dest[3] = addr2[0];
  dest[4] = addr2[1];
  dest[5] = addr2[2];
}
