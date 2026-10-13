#include "sdk/tp.h"
#include "sdk/pxi.h"

void TPi_TpCallback(u32 tag, u32 data,
                    int err); /* the touch panel PXI receive callback */

static u16 tpInitialized; /* TRUE once TP_Init has run */
TPiWork tpState;

/* Clears the touch panel state and, once the ARM7 driver is up, installs
 * the PXI callback that receives its answers. Runs only once. */
void TP_Init(void) {
  if (tpInitialized) {
    return;
  }
  tpInitialized = TRUE;

  PXI_Init();
  tpState.state = TP_STATE_READY;
  tpState.command_flg = 0;
  tpState.index = 0;
  tpState.callback = NULL;
  tpState.samplingBufs = NULL;
  tpState.calibrate_flg = FALSE;
  tpState.err_flg = 0;

  while (!PXI_IsCallbackReady(FIFO_TAG_TOUCH, PXI_PROC_ARM7)) {
  }
  PXI_SetFifoRecvCallback(FIFO_TAG_TOUCH, TPi_TpCallback);
}
