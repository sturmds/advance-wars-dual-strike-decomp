#include "types.h"
#include "sdk/wm.h"

/* Parent side: chooses whether new children may join. */
int WM_SetEntry(WMCallbackFunc callback, int entryFlag)
{
    int result = WMi_CheckStateEx(2, WM_STATE_PARENT, WM_STATE_MP_PARENT);

    if (result != 0) {
        return result;
    }
    func_02116f74(WM_APIID_SET_ENTRY, callback);
    result = WMi_SendCommand(WM_APIID_SET_ENTRY, 1, entryFlag);
    if (result == 0) {
        result = WM_ERRCODE_OPERATING;
    }
    return result;
}
