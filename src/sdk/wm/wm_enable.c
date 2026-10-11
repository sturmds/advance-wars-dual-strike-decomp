#include "types.h"
#include "sdk/wm.h"

/* Starts the wireless hardware's driver on the ARM7; WM goes from READY to
 * STOP. */
int WM_Enable(WMCallbackFunc callback)
{
    WMArm9Buf *buf;
    int result = WMi_CheckStateEx(1, WM_STATE_READY);

    if (result != 0) {
        return result;
    }
    func_02116f74(WM_APIID_ENABLE, callback);
    buf = func_02116e10();
    result = WMi_SendCommand(WM_APIID_ENABLE, 3, buf->arm7Buf, buf->status, buf->replyBuf);
    if (result == 0) {
        result = WM_ERRCODE_OPERATING;
    }
    return result;
}
