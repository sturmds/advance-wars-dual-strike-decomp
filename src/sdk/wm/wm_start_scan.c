#include "types.h"
#include "sdk/wm.h"

/* Listens on one channel for parents' beacons. A parent found is written to
 * param->scanBuf and reported to the callback. */
int WM_StartScan(WMCallbackFunc callback, const WMScanParam *param)
{
    WMStartScanReq *req;
    int result = WMi_CheckStateEx(3, WM_STATE_IDLE, WM_STATE_CLASS1, WM_STATE_SCAN);

    if (result != 0) {
        return result;
    }
    if (param == NULL) {
        return WM_ERRCODE_INVALID_PARAM;
    }
    if (param->scanBuf == NULL) {
        return WM_ERRCODE_INVALID_PARAM;
    }
    if (param->channel < 1 || param->channel > 14) {
        return WM_ERRCODE_INVALID_PARAM;
    }
    func_02116f74(WM_APIID_START_SCAN, callback);
    req = func_02116e10()->requestBuf;
    req->apiid = WM_APIID_START_SCAN;
    req->channel = param->channel;
    req->scanBuf = param->scanBuf;
    req->maxChannelTime = param->maxChannelTime;
    req->bssid[0] = param->bssid[0];
    req->bssid[1] = param->bssid[1];
    req->bssid[2] = param->bssid[2];
    req->bssid[3] = param->bssid[3];
    req->bssid[4] = param->bssid[4];
    req->bssid[5] = param->bssid[5];
    result = func_02116e20(WM_APIID_START_SCAN, 0);
    if (result == 0) {
        result = WM_ERRCODE_OPERATING;
    }
    return result;
}
