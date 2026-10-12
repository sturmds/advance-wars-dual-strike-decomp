#include "types.h"
#include "sdk/os.h"
#include "sdk/pxi.h"

extern u16 gOsResetInitialized;

void OS_InitReset(void)
{
    if (gOsResetInitialized != 0) {
        return;
    }
    gOsResetInitialized = 1;
    PXI_Init();
    while (PXI_IsCallbackReady(FIFO_TAG_RESET, 1) == 0) {
    }
    PXI_SetFifoRecvCallback(FIFO_TAG_RESET, Reset_OnOsTagMessage);
}
