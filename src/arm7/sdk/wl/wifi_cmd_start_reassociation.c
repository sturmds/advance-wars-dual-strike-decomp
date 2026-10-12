#include "sdk/wl.h"

void WifiMlme_RunReassociation(void);

// Sends an 802.11 reassociation request to the access point at request->mac.
// The reply goes out when the response arrives or the timeout runs out.
int WifiCmd_StartReassociation(WlAssociateRequest *request, WlReply *reply)
{
    WlWork *work = WIFI_DRIVER_STATE;
    WlLink *link = WIFI_LINK_VIEW_OF(work);
    WlPendingRequest *pending = WIFI_WAITING_REQUEST_OF;

    reply->status = 3;
    if (work->state.phase != 3 && work->state.phase != 2) {
        return 11;
    }
    if (link->mode < 0x30) {
        return 1;
    }
    if (request->mac[0] & 1) {
        return WIFI_ANSWER_OUT_OF_RANGE;
    }
    if (request->listenInterval < 1) {
        return WIFI_ANSWER_OUT_OF_RANGE;
    }
    if (request->listenInterval > 255) {
        return WIFI_ANSWER_OUT_OF_RANGE;
    }
    if (request->timeoutMs > 2000) {
        return WIFI_ANSWER_OUT_OF_RANGE;
    }
    if (request->timeoutMs < 10) {
        return WIFI_ANSWER_OUT_OF_RANGE;
    }
    link->listenInterval = request->listenInterval;
    link->unk_72 = request->listenInterval;
    pending->request = request;
    pending->reply = reply;
    pending->command = 0x60;
    WifiMlme_RunReassociation();
    return WIFI_ANSWER_LATER;
}
