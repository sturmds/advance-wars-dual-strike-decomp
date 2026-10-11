#include "sdk/tp.h"

void func_02110e58(void);                        /* likely PXI_Init */
int func_0211103c(u32 tag, u32 proc);            /* likely PXI_IsCallbackReady */
void func_02111064(u32 tag, void (*callback)(u32, u32, int));   /* likely PXI_SetFifoRecvCallback */
void TPi_TpCallback(u32 tag, u32 data, int err);  /* the touch panel PXI receive callback */

extern u16 tpInitialized;                        /* TRUE once TP_Init has run */

#define PXI_PROC_ARM7 1

/* Clears the touch panel state and, once the ARM7 driver is up, installs
 * the PXI callback that receives its answers. Runs only once. */
void TP_Init(void)
{
    if (tpInitialized) {
        return;
    }
    tpInitialized = TRUE;

    func_02110e58();
    tpState.state = TP_STATE_READY;
    tpState.command_flg = 0;
    tpState.index = 0;
    tpState.callback = NULL;
    tpState.samplingBufs = NULL;
    tpState.calibrate_flg = FALSE;
    tpState.err_flg = 0;

    while (!func_0211103c(FIFO_TAG_TOUCH, PXI_PROC_ARM7)) {
    }
    func_02111064(FIFO_TAG_TOUCH, TPi_TpCallback);
}
