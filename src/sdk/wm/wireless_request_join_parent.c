#include "types.h"
#include "sdk/wm.h"

void DC_StoreRange(void *start, u32 size);
void Mem_CopyBytes(const void *src, void *dest, u32 size);
void MI_CpuFill8(void *dest, u8 value, u32 size);

/* Child side: asks to join the parent described by bssDesc, which a scan
 * found. ssid is the 24-byte child name, or NULL for none. */
int Wireless_RequestJoinParent(WMCallbackFunc callback, WMBssDesc *bssDesc, const u8 *ssid, int powerSave,
                    u16 authMode)
{
    WMStartConnectReq *req;
    int result = WMi_CheckStateEx(1, WM_STATE_IDLE);

    if (result != 0) {
        return result;
    }
    if (bssDesc == NULL) {
        return WM_ERRCODE_INVALID_PARAM;
    }
    DC_StoreRange(bssDesc, bssDesc->length * 2);
    func_02116f74(WM_APIID_START_CONNECT, callback);
    req = func_02116e10()->requestBuf;
    req->apiid = WM_APIID_START_CONNECT;
    req->pInfo = bssDesc;
    if (ssid != NULL) {
        Mem_CopyBytes(ssid, req->ssid, 24);
    } else {
        MI_CpuFill8(req->ssid, 0, 24);
    }
    req->powerSave = powerSave;
    req->authMode = authMode;
    result = WMi_SendCommand(WM_APIID_START_CONNECT, 0);
    if (result == 0) {
        result = WM_ERRCODE_OPERATING;
    }
    return result;
}
