#include "types.h"
#include "sdk/ctrdg.h"
#include "sdk/pxi.h"

/* also declared in src/sdk/ctrdg/ctrdgi_pxi_callback.c */
int (*gCtrdgPulledOutHook)(void);
static u32 gCtrdgInitialized;
/* aligned to 32: no symbol covers the bytes before it */
__attribute__((aligned(32))) GbaRomHeader data_022acf40;

void CTRDG_Init(void) {
  if (gCtrdgInitialized != 0) {
    return;
  }
  gCtrdgInitialized = 1;
  Cartridge_ZeroStateTakeLockId();
  PXI_Init();
  while (PXI_IsCallbackReady(FIFO_TAG_GBA_SLOT, 1) == 0) {
  }
  PXI_SetFifoRecvCallback(FIFO_TAG_GBA_SLOT, Cartridge_OnInitDoneMessage);
  Cartridge_CopyGbaHeaderOnce();
  PXI_SetFifoRecvCallback(FIFO_TAG_GBA_SLOT, NULL);
  PXI_SetFifoRecvCallback(FIFO_TAG_GBA_SLOT, Cartridge_OnRemovalMessage);
  gCtrdgPulledOutHook = NULL;
}
