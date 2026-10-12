#include "types.h"
#include "sdk/wm.h"
#include "sdk/os.h"

/* Drops a connection. A parent names the child's aid (1 to 15); a child
 * passes 0 to leave its parent. */
int WM_Disconnect(WMCallbackFunc callback, u16 aid)
{
    WMArm9Buf *buf = func_02116e10();
    int result;

    result = WMi_CheckStateEx(5, WM_STATE_PARENT, WM_STATE_MP_PARENT, WM_STATE_CHILD,
                           WM_STATE_MP_CHILD, WM_STATE_DCF_CHILD);
    if (result != 0) {
        return result;
    }
    if (buf->status->state == WM_STATE_PARENT || buf->status->state == WM_STATE_MP_PARENT) {
        if (aid < 1 || aid > 15) {
            return WM_ERRCODE_INVALID_PARAM;
        }
        DC_InvalidateRange(&buf->status->childBitmap, 2);
        if ((buf->status->childBitmap & (1 << aid)) == 0) {
            return WM_ERRCODE_NO_CHILD;
        }
    } else if (aid != 0) {
        return WM_ERRCODE_INVALID_PARAM;
    }
    func_02116f74(WM_APIID_DISCONNECT, callback);
    result = WMi_SendCommand(WM_APIID_DISCONNECT, 1, 1 << aid);
    if (result == 0) {
        result = WM_ERRCODE_OPERATING;
    }
    return result;
}
