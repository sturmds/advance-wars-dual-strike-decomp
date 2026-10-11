#include "types.h"
#include "sdk/wm.h"

/* Stops the scan started by WM_StartScan. */
int WM_EndScan(WMCallbackFunc callback)
{
    int result = WMi_CheckStateEx(1, WM_STATE_SCAN);

    if (result != 0) {
        return result;
    }
    func_02116f74(WM_APIID_END_SCAN, callback);
    result = func_02116e20(WM_APIID_END_SCAN, 0);
    if (result == 0) {
        result = WM_ERRCODE_OPERATING;
    }
    return result;
}
