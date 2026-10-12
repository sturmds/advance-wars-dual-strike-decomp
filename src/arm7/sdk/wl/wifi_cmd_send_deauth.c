#include "sdk/wl.h"

void func_arm7_027c90ec(u16 *dst, const u16 *src); /* copies a 6-byte MAC address */
WlTxFrame *WifiFrame_NewDeauthentication(const u16 *dest, u32 reason);
void WifiTx_EnqueueMgmt(WlTxFrame *frame);
void func_arm7_027d03c0(const u16 *mac);
void func_arm7_027d1034(void);
void WifiTx_EnqueueGroupcast(WlBufferList *from, WlBuffer *buffer);

// Sends an 802.11 deauthentication frame to request->mac, to every station
// when the address is multicast (refused in phases 2 and 3). The reply echoes
// the address and goes out once the frame is sent.
int WifiCmd_SendDeauth(WlMacRequest *request, WlMacReply *reply)
{
    WlWork *work;
    u16 phase;
    WlTxFrame *frame;

    reply->status = 4;
    work = WIFI_DRIVER_STATE;
    phase = work->state.phase;
    if (phase != 3 && phase != 2 && phase != 1) {
        return 11;
    }
    if (work->state.mode < 0x30) {
        return 1;
    }
    if ((phase == 2 || phase == 3) && (request->mac[0] & 1)) {
        return WIFI_ANSWER_OUT_OF_RANGE;
    }
    func_arm7_027c90ec(reply->mac, request->mac);
    frame = WifiFrame_NewDeauthentication(reply->mac, request->reason);
    if (frame == NULL) {
        return WIFI_ANSWER_NO_MEMORY;
    }
    WIFI_ACTIVE_JOB_OF(WIFI_DRIVER_STATE)->any.request = request;
    WIFI_ACTIVE_JOB_OF(WIFI_DRIVER_STATE)->any.reply = (WlReply *)reply;
    WIFI_ACTIVE_JOB_OF(WIFI_DRIVER_STATE)->any.state = 0x41;
    if (request->mac[0] & 1) {
        WifiTx_EnqueueGroupcast(&WIFI_DRIVER_STATE->unk_15c, WIFI_HOST_TX_HEADER_OF(frame));
        if ((WIFI_DRIVER_STATE->stationBits.unk_02 & ~WIFI_DRIVER_STATE->stationBits.unk_06) == 0) {
            func_arm7_027d1034();
        }
    } else {
        func_arm7_027d03c0(request->mac);
        WifiTx_EnqueueMgmt(frame);
    }
    return WIFI_ANSWER_LATER;
}
