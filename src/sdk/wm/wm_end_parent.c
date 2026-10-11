#include "types.h"
#include "sdk/wm.h"

/* Stops acting as a parent. */
int WM_EndParent(WMCallbackFunc callback)
{
    int result = WMi_CheckStateEx(1, WM_STATE_PARENT);

    if (result != 0) {
        return result;
    }
    func_02116f74(WM_APIID_END_PARENT, callback);
    result = WMi_SendCommand(WM_APIID_END_PARENT, 0);
    if (result == 0) {
        result = WM_ERRCODE_OPERATING;
    }
    return result;
}
