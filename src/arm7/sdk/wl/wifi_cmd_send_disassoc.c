#include "sdk/wl.h"

WlTxFrame *WifiFrame_NewDisassociation(const u16 *dest, u32 reason);
void func_arm7_027d01c4(WlTxFrame *frame);
void func_arm7_027d03c0(const u16 *mac);
void func_arm7_027d1034(void);
void WifiTx_EnqueueGroupcast(WlBufferList *from, WlBuffer *buffer);

// Sends an 802.11 disassociation frame to request->mac, to every station when
// the address is multicast (allowed only in phase 1). The reply goes out once
// the frame is sent.
int WifiCmd_SendDisassoc(WlMacRequest *request, WlReply *reply)
{
    WlPendingRequest *pending = WIFI_WAITING_REQUEST_OF;
    WlTxFrame *frame;

    reply->status = 1;
    if (WIFI_DRIVER_STATE->state.phase == 0) {
        return 11;
    }
    if (WIFI_DRIVER_STATE->state.phase != 1 && (request->mac[0] & 1)) {
        return WIFI_ANSWER_OUT_OF_RANGE;
    }
    if (WIFI_DRIVER_STATE->state.mode != 0x40) {
        return 1;
    }
    frame = WifiFrame_NewDisassociation(request->mac, request->reason);
    if (frame == NULL) {
        return WIFI_ANSWER_NO_MEMORY;
    }
    pending->request = request;
    pending->reply = reply;
    pending->command = 0x71;
    if (request->mac[0] & 1) {
        WifiTx_EnqueueGroupcast(&WIFI_DRIVER_STATE->unk_15c, WIFI_HOST_TX_HEADER_OF(frame));
        if ((WIFI_DRIVER_STATE->stationBits.unk_02 & ~WIFI_DRIVER_STATE->stationBits.unk_06) == 0) {
            func_arm7_027d1034();
        }
    } else {
        func_arm7_027d03c0(request->mac);
        func_arm7_027d01c4(frame);
    }
    return WIFI_ANSWER_LATER;
}
