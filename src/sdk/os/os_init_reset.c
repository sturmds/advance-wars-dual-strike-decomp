#include "types.h"
#include "sdk/os.h"

void func_02110e58(void);   /* called with no arguments; src/sdk/func_02110e58.c declares three */
int PXI_IsCallbackReady(u32 tag, u32 proc);   /* likely PXI_IsCallbackReady: nonzero once the ARM7 side is ready */
void PXI_SetFifoRecvCallback(u32 tag, void (*callback)(u32, u32, int));   /* likely PXI_SetFifoRecvCallback */

extern u16 gOsResetInitialized;

void OS_InitReset(void)
{
    if (gOsResetInitialized != 0) {
        return;
    }
    gOsResetInitialized = 1;
    func_02110e58();
    while (PXI_IsCallbackReady(FIFO_TAG_RESET, 1) == 0) {
    }
    PXI_SetFifoRecvCallback(FIFO_TAG_RESET, Reset_OnOsTagMessage);
}
