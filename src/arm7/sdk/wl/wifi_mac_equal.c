#include "sdk/wl.h"

BOOL WifiMac_Equal(const u16* a, const u16* b) {
  if (a[2] == b[2] && a[1] == b[1] && a[0] == b[0]) {
    return TRUE;
  }
  return FALSE;
}
