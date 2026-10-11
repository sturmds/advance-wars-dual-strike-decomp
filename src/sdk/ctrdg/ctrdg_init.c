#include "types.h"
#include "sdk/ctrdg.h"

void func_02110e58(void);   /* called with no arguments; src/sdk/func_02110e58.c declares three */
int PXI_IsCallbackReady(u32 tag, u32 proc);   /* likely PXI_IsCallbackReady: nonzero once the ARM7 side is ready */
void PXI_SetFifoRecvCallback(u32 tag, void (*callback)(u32, u32, int));   /* likely PXI_SetFifoRecvCallback */

extern u32 gCtrdgInitialized;
extern int (*gCtrdgPulledOutHook)(void);

void CTRDG_Init(void)
{
    if (gCtrdgInitialized != 0) {
        return;
    }
    gCtrdgInitialized = 1;
    Cartridge_ZeroStateTakeLockId();
    func_02110e58();
    while (PXI_IsCallbackReady(FIFO_TAG_GBA_SLOT, 1) == 0) {
    }
    PXI_SetFifoRecvCallback(FIFO_TAG_GBA_SLOT, Cartridge_OnInitDoneMessage);
    Cartridge_CopyGbaHeaderOnce();
    PXI_SetFifoRecvCallback(FIFO_TAG_GBA_SLOT, NULL);
    PXI_SetFifoRecvCallback(FIFO_TAG_GBA_SLOT, Cartridge_OnRemovalMessage);
    gCtrdgPulledOutHook = NULL;
}
