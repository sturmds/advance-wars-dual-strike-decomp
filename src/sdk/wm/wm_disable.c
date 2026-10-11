#include "types.h"
#include "sdk/wm.h"

/* Releases the wireless hardware; WM goes back to READY. */
int WM_Disable(WMCallbackFunc callback)
{
    int result = WMi_CheckStateEx(1, WM_STATE_STOP);

    if (result != 0) {
        return result;
    }
    func_02116f74(WM_APIID_DISABLE, callback);
    result = WMi_SendCommand(WM_APIID_DISABLE, 0);
    if (result == 0) {
        result = WM_ERRCODE_OPERATING;
    }
    return result;
}
