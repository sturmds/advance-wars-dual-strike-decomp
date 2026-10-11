#include "types.h"
#include "sdk/wm.h"

/* Starts acting as a parent: sends beacons and accepts children. */
int WM_StartParent(WMCallbackFunc callback)
{
    int result = WMi_CheckStateEx(1, WM_STATE_IDLE);

    if (result != 0) {
        return result;
    }
    func_02116f74(WM_APIID_START_PARENT, callback);
    result = func_02116e20(WM_APIID_START_PARENT, 0);
    if (result == 0) {
        result = WM_ERRCODE_OPERATING;
    }
    return result;
}
