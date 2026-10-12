#include "sdk/wl.h"

void Wifi_SwitchMode(int mode); /* switches WlLink.mode */
void func_arm7_027c90ec(u16 *dst, const u16 *src); /* copies a 6-byte MAC address */
void WifiMlme_RunAuthentication(void);

// Switches to mode 0x20 and sends an 802.11 authentication frame to the
// access point at request->mac. The reply echoes the address and algorithm
// and goes out when the response arrives or the timeout runs out.
int WifiCmd_StartAuthentication(WlAuthenticateRequest *request, WlAuthenticateReply *reply)
{
    WlWork *work = WIFI_DRIVER_STATE;
    WlLink *link = WIFI_LINK_VIEW_OF(work);
    WlPendingRequest *pending = WIFI_WAITING_REQUEST_OF;

    reply->status = 6;
    if (work->state.phase != 3 && work->state.phase != 2) {
        return 11;
    }
    if (link->mode < 0x20) {
        return 1;
    }
    if (request->mac[0] & 1) {
        return WIFI_ANSWER_OUT_OF_RANGE;
    }
    if (request->algorithm > 1) {
        return WIFI_ANSWER_OUT_OF_RANGE;
    }
    if (request->timeoutMs > 2000) {
        return WIFI_ANSWER_OUT_OF_RANGE;
    }
    if (request->timeoutMs < 10) {
        return WIFI_ANSWER_OUT_OF_RANGE;
    }
    Wifi_SwitchMode(0x20);
    pending->request = request;
    pending->reply = (WlReply *)reply;
    pending->command = 0x30;
    /* needed to match: the original reads the request back from the pending slot */
    reply->algorithm = ((WlAuthenticateRequest *)pending->request)->algorithm;
    func_arm7_027c90ec(reply->mac, ((WlAuthenticateRequest *)pending->request)->mac);
    WifiMlme_RunAuthentication();
    return WIFI_ANSWER_LATER;
}
