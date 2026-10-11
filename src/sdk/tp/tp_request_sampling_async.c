#include "sdk/tp.h"
#include "sdk/os.h"

int PXI_SendWordByFifo(u32 tag, u32 data, int err);   /* likely PXI_SendWordByFifo: negative when the FIFO is full */

static inline BOOL Touch_PostSampleRequest(void)
{
    if (PXI_SendWordByFifo(FIFO_TAG_TOUCH, TOUCH_ASK_ONE_SAMPLE, 0) < 0) {
        return FALSE;
    }
    return TRUE;
}

/* Asks the ARM7 for one touch panel sample; TP_WaitRawResult collects it.
 * When the PXI FIFO is full the request fails at once and the callback, if
 * any, gets TP_RESULT_PXI_BUSY. */
void TP_RequestSamplingAsync(void)
{
    int enabled = OS_DisableInterrupts();

    if (!Touch_PostSampleRequest()) {
        OS_RestoreInterrupts(enabled);
        tpState.err_flg |= TOUCH_JOB_ONE_SAMPLE_BIT;
        if (tpState.callback != NULL) {
            tpState.callback(TP_REQUEST_COMMAND_SAMPLING, TP_RESULT_PXI_BUSY, 0);
        }
        return;
    }
    tpState.command_flg |= TOUCH_JOB_ONE_SAMPLE_BIT;
    tpState.err_flg &= ~TOUCH_JOB_ONE_SAMPLE_BIT;
    OS_RestoreInterrupts(enabled);
}
