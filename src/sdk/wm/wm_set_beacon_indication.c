#include "types.h"
#include "sdk/wm.h"

/* Turns the indication that the ARM7 raises for each beacon it receives on
 * (1) or off (0). */
int WM_SetBeaconIndication(WMCallbackFunc callback, int flag)
{
    int result = WMi_CheckIdle();

    if (result != 0) {
        return result;
    }
    if (flag != 0 && flag != 1) {
        return WM_ERRCODE_INVALID_PARAM;
    }
    func_02116f74(WM_APIID_SET_BEACON_IND, callback);
    result = WMi_SendCommand(WM_APIID_SET_BEACON_IND, 1, flag);
    if (result == 0) {
        result = WM_ERRCODE_OPERATING;
    }
    return result;
}
