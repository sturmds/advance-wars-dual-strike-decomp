#include "types.h"

/* Checks that the send size (0xe4 to 0x1fe), mpParam (8 to 16) and the child
 * count give a buffer under 0x15e0 bytes. Returns 1 when they fit. */
int MbParent_CheckCommParam(u32 sendSize, u32 mpParam, int maxChildren) {
  int bufferSize;

  if (sendSize > 0x1fe || sendSize < 0xe4) {
    return 0;
  }
  if (mpParam > 16 || mpParam < 8) {
    return 0;
  }
  bufferSize =
      0x14a + (sendSize + 38) * 4 + ((mpParam + 32) * 4 + 0x70) * maxChildren;
  return bufferSize < 0x15e0;
}
