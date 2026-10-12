#include "types.h"
#include "sdk/pxi.h"

/* TRUE once CPU proc has registered a receive callback for tag. The local
 * pointer is needed to match: it keeps the 0x388 offset out of the address. */
BOOL PXI_IsCallbackReady(u32 tag, u32 proc) {
  BOOL ready = TRUE;
  volatile FifoPeerRegistry* work = FIFO_PEER_REGISTRY;

  if (!(work->callbackFlags[proc] & (1 << tag))) {
    ready = FALSE;
  }
  return ready;
}
