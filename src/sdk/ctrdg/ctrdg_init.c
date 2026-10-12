#include "types.h"
#include "sdk/ctrdg.h"
#include "sdk/pxi.h"

extern u32 gCtrdgInitialized;
extern int (*gCtrdgPulledOutHook)(void);

void CTRDG_Init(void)
{
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
