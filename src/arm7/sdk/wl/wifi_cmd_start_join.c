#include "sdk/wl.h"

void Wifi_SwitchMode(int mode); /* switches WlLink.mode */
u32 WifiFlash_ValidateRadioBlock(u32 *crcs);
int WifiParam_SetShortPreamble(u32 enable);
int WifiParam_LoadBssid(const u16 *bssid);
int WifiParam_StoreSsid(u32 length, const u8 *src);
int WifiParam_ChangeBeaconPeriod(u16 interval);
int WifiRf_TuneChannel(u16 channel, int fromFlash);
int WifiParam_StoreRates(const WlRateSet *rates);

// Checks the BSS description in the request, loads its BSSID, SSID, beacon
// interval, channel and rates, and queues task 1 to join it. The reply goes
// out when the join ends.
int WifiCmd_StartJoin(WlJoinRequest *request, WlReply *reply)
{
    WlWork *work = WIFI_DRIVER_STATE;
    WlLink *link = WIFI_LINK_VIEW_OF(work);
    WlMlmeJob *job = &WIFI_ACTIVE_JOB_OF(work)->any;

    reply->status = 5;
    if (work->state.phase != 3 && work->state.phase != 2) {
        return 11;
    }
    if (link->mode < 0x20) {
        return 1;
    }
    Wifi_SwitchMode(0x20);
    if (request->bssid[0] & 1) {
        return WIFI_ANSWER_OUT_OF_RANGE;
    }
    if (request->ssidLength == 0) {
        return WIFI_ANSWER_OUT_OF_RANGE;
    }
    if (request->ssidLength > WIFI_SSID_LONGEST) {
        return WIFI_ANSWER_OUT_OF_RANGE;
    }
    if (request->beaconInterval < 10) {
        return WIFI_ANSWER_OUT_OF_RANGE;
    }
    if (request->beaconInterval > 1000) {
        return WIFI_ANSWER_OUT_OF_RANGE;
    }
    if (request->dtimPeriod > 255) {
        return WIFI_ANSWER_OUT_OF_RANGE;
    }
    if (request->channel & 0xfff0) {
        return WIFI_ANSWER_OUT_OF_RANGE;
    }
    if (!WifiParam_ChannelAllowed(request->channel)) {
        return WIFI_ANSWER_OUT_OF_RANGE;
    }
    if (request->rates.basic & ~0xfff) {
        return WIFI_ANSWER_OUT_OF_RANGE;
    }
    if (request->rates.supported & ~0xfff) {
        return WIFI_ANSWER_OUT_OF_RANGE;
    }
    if (request->rates.basic == 0) {
        return WIFI_ANSWER_OUT_OF_RANGE;
    }
    if ((request->rates.supported | request->rates.basic) == 0) {
        return WIFI_ANSWER_OUT_OF_RANGE;
    }
    if (request->timeoutMs > 2000) {
        return WIFI_ANSWER_OUT_OF_RANGE;
    }
    if (WifiFlash_ValidateRadioBlock(NULL) != WIFI_RADIO_BLOCK_VALID) {
        return WIFI_ANSWER_FLASH_BLOCK_BAD;
    }
    /* capability bit 5: the BSS allows short preambles */
    if (!(request->capability & 0x20)) {
        WifiParam_SetShortPreamble(0);
    }
    WifiParam_LoadBssid(request->bssid);
    WifiParam_StoreSsid(request->ssidLength, request->ssid);
    WifiParam_ChangeBeaconPeriod(request->beaconInterval);
    WifiRf_TuneChannel(request->channel, 0);
    WifiParam_StoreRates(&request->rates);
    job->request = request;
    job->reply = reply;
    job->state = 0x20;
    WifiTask_Schedule(2, 1);
    return WIFI_ANSWER_LATER;
}
