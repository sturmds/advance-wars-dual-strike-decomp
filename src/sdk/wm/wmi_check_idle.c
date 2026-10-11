#include "sdk/wm.h"

void DC_InvalidateRange(void *start, u32 size);

extern WMArm9Buf *data_022ac32c;

int WMi_CheckIdle(void)
{
    int result;

    result = func_02116df4();
    if (result != WM_ERRCODE_SUCCESS) {
        return result;
    }

    DC_InvalidateRange(&data_022ac32c->status->state, 2);
    if (data_022ac32c->status->state == WM_STATE_READY || data_022ac32c->status->state == WM_STATE_STOP) {
        return WM_ERRCODE_ILLEGAL_STATE;
    }
    return WM_ERRCODE_SUCCESS;
}
