#include "sdk/wm.h"

void WmClearFifoRecvFlag(void);
void func_02111064(int tag, void *callback);

extern u16 data_022ac328;
extern WMArm9Buf *data_022ac32c;

/* Undoes WM_Init: stops taking WM messages from the ARM7. */
int WM_Finish(void)
{
    int result;

    result = func_02116df4();
    if (result != WM_ERRCODE_SUCCESS) {
        return WM_ERRCODE_ILLEGAL_STATE;
    }
    result = WMi_CheckStateEx(1, WM_STATE_READY);
    if (result != WM_ERRCODE_SUCCESS) {
        return result;
    }

    WmClearFifoRecvFlag();
    func_02111064(PXI_FIFO_TAG_WM, NULL);
    data_022ac32c = NULL;
    data_022ac328 = FALSE;
    return WM_ERRCODE_SUCCESS;
}
