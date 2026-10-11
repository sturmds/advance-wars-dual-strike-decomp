#include "types.h"
#include "sdk/wm.h"

void DC_InvalidateRange(void *start, u32 size);

/* Stops the MP exchange of a parent or child. */
int WM_EndMP(WMCallbackFunc callback)
{
    WMArm9Buf *buf = func_02116e10();
    int result = WMi_CheckStateEx(2, WM_STATE_MP_PARENT, WM_STATE_MP_CHILD);

    if (result != 0) {
        return result;
    }
    DC_InvalidateRange(&buf->status->unk_0c, 4);
    if (buf->status->unk_0c == 0) {
        return WM_ERRCODE_ILLEGAL_STATE;
    }
    func_02116f74(WM_APIID_END_MP, callback);
    result = WMi_SendCommand(WM_APIID_END_MP, 0);
    if (result == 0) {
        result = WM_ERRCODE_OPERATING;
    }
    return result;
}
