#include "types.h"
#include "sdk/wm.h"

void DC_StoreRange(void *start, u32 size);

/* Gives the ARM7 the settings to use when this DS acts as a parent. */
int WM_SetParentParameter(WMCallbackFunc callback, WMParentParam *param)
{
    int result = WMi_CheckStateEx(1, WM_STATE_IDLE);

    if (result != 0) {
        return result;
    }
    if (param == NULL) {
        return WM_ERRCODE_INVALID_PARAM;
    }
    if (param->userGameInfoLength != 0 && param->userGameInfo == NULL) {
        return WM_ERRCODE_INVALID_PARAM;
    }
    if (param->parentMaxSize + (param->KS_Flag != 0 ? 42 : 0) > 512
        || param->childMaxSize + (param->KS_Flag != 0 ? 6 : 0) > 512) {
        return WM_ERRCODE_INVALID_PARAM;
    }
    /* The original ignores the result of this range check. */
    Wireless_ParentSettingsValid(param);
    func_02116f74(WM_APIID_SET_P_PARAM, callback);
    DC_StoreRange(param, sizeof(WMParentParam));
    if (param->userGameInfoLength != 0) {
        DC_StoreRange(param->userGameInfo, param->userGameInfoLength);
    }
    result = WMi_SendCommand(WM_APIID_SET_P_PARAM, 1, param);
    if (result == 0) {
        result = WM_ERRCODE_OPERATING;
    }
    return result;
}
