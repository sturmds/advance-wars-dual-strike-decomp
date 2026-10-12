#include "sdk/wl.h"

u8 WL_ReadByte(const void *addr);
int WifiParam_LoadBssid(const u16 *bssid);
int WifiParam_StoreSsid(u32 length, const u8 *src);

// Checks a scan request (SSID, scan type, channel list, time per channel),
// loads its BSSID and SSID filter and queues task 0 to scan. The reply goes
// out when the scan ends.
int WifiCmd_StartScan(WlScanRequest *request, WlReply *reply)
{
    WlWork *work = WIFI_DRIVER_STATE;
    WlPendingRequest *pending = WIFI_WAITING_REQUEST_OF;
    u32 i;
    u8 channel;

    pending->unk_04 = reply->status - 2;
    reply->status = 3;
    if (work->state.phase != 1 && work->state.phase != 3 && work->state.phase != 2) {
        return 11;
    }
    /* needed to match: the original reloads WIFI_DRIVER_STATE here */
    if (WIFI_DRIVER_STATE->state.mode < 0x20) {
        return 1;
    }
    if (request->ssidLength > WIFI_SSID_LONGEST) {
        return WIFI_ANSWER_OUT_OF_RANGE;
    }
    if (request->unk_38 > 1) {
        return WIFI_ANSWER_OUT_OF_RANGE;
    }
    if (WL_ReadByte(&request->channels[0]) == 0) {
        return WIFI_ANSWER_OUT_OF_RANGE;
    }
    if (request->channelTimeMs > 1000) {
        return WIFI_ANSWER_OUT_OF_RANGE;
    }
    if (request->channelTimeMs < 10) {
        return WIFI_ANSWER_OUT_OF_RANGE;
    }
    if (request->unk_4c > 16) {
        return WIFI_ANSWER_OUT_OF_RANGE;
    }
    for (i = 0; i < sizeof(request->channels); i++) {
        channel = WL_ReadByte(&request->channels[i]);
        if (channel == 0) {
            break;
        }
        if (!WifiParam_ChannelAllowed(channel)) {
            return WIFI_ANSWER_OUT_OF_RANGE;
        }
    }
    WifiParam_LoadBssid(request->peer);
    WifiParam_StoreSsid(request->ssidLength, request->ssid);
    pending->request = request;
    pending->reply = reply;
    pending->command = 0x10;
    WifiTask_Schedule(2, 0);
    return WIFI_ANSWER_LATER;
}
