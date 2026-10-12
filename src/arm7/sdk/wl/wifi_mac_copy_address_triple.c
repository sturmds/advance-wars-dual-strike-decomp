#include "sdk/wl.h"

// Copies three 6-byte MAC addresses into consecutive slots of dest.
void WifiMac_CopyAddressTriple(u16* dest, const u16* addr1, const u16* addr2,
                               const u16* addr3) {
  dest[0] = addr1[0];
  dest[1] = addr1[1];
  dest[2] = addr1[2];
  dest[3] = addr2[0];
  dest[4] = addr2[1];
  dest[5] = addr2[2];
  dest[6] = addr3[0];
  dest[7] = addr3[1];
  dest[8] = addr3[2];
}
