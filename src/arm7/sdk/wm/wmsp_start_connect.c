#include "sdk/wmsp.h"
#include "sdk/os_arm7.h"
#include "sdk/mi.h"

/* WL command ids, named after what the handler does around them. */
#define WIFI_REQ_DOZE_SETTING 1             /* takes WMStartConnectReq.powerSave; WMStatus.unk_c2 keeps it */
#define WIFI_REQ_SYNC_TO_PARENT 3                       /* takes the parent's WlJoinBssDesc, replies with its MAC address */
#define WIFI_REQ_AUTH 4               /* takes the parent's MAC address and the auth mode */
#define WIFI_REQ_ASSOC 6                  /* takes the parent's MAC address, replies with our aid */
#define WIFI_REQ_ENTER_READY 0x303              /* WMStatus.state becomes WM_STATE_CLASS1 after it */

#define WIFI_JOIN_STEP_WAIT_MS 2000

/* The reply the WL gives when the parent turns this child away; WM reports
   it as WM_ERRCODE_OVER_MAX_ENTRY. */
#define WIFI_ANSWER_PEER_SAID_NO 12
#define WIFI_JOIN_REFUSED_FULL 19

#define WIRELESS_JOINABLE_CHANNELS 0x1fff              /* channels 1 to 13, bit 0 for channel 1 */

/* IEEE 802.11 capability bits in WMBssDesc.capaInfo */
#define CAPA_PRIVACY 0x10
#define CAPA_SHORT_PREAMBLE 0x20

u16 *func_arm7_027c1ee4(WlCommand *cmd);
u16 *func_arm7_027c2890(WlCommand *cmd, int arg0, int arg1, int arg2);
u16 *func_arm7_027c2750(WlCommand *cmd, int arg, const void *src);
u16 *func_arm7_027c26cc(WlCommand *cmd, const void *src, int arg0, int arg1);
u16 *func_arm7_027c25c8(WlCommand *cmd, const void *src, int arg0, int arg1);
u8 Wmsp_DecodeRssi(int rawRssi);
int Wmsp_RssiToLinkLevel(u32 rssi);
void Wmsp_ResetRssiHistory(u8 rssi);

/* The parent this child connects to, kept in WmspBuf. */
#define PARENT WMSP_BSS_DESC(buf)

/* Joins the parent in the request's WMBssDesc: join, authenticate, then
   associate. The ARM9 gets a CONNECT_START reply once the request is
   accepted and a CONNECTED reply with the aid when it is done; WM then runs
   in WM_STATE_CHILD. */
void WMSP_StartConnect(WMStartConnectReq *req)
{
    u32 wlBuf[WMSP_WL_BUF_WORDS];               /* needed to match: word aligned */
    WlCommand *cmd = (WlCommand *)wlBuf;
    WmspBuf *buf = func_arm7_027c05e0()->buf;
    volatile u16 one;                           /* needed to match: the 1 goes through the stack */
    u8 authMac[6];
    u8 assocMac[6];
    WlJoinBssDesc join;
    WMStatus *status;
    WMStartConnectCallback *reply;
    u16 *wlReply;
    int channelBit;
    u16 powerSave;
    int intr;
    int intr2;
    u8 rssi;

    if (buf->status->state != WM_STATE_IDLE) {
        reply = Wmsp_GetCallbackBuffer();
        reply->apiid = WM_APIID_START_CONNECT;
        reply->errcode = WM_ERRCODE_ILLEGAL_STATE;
        reply->state = WM_STATECODE_CONNECT_START;
        Wmsp_SendReply(reply);
        return;
    }
    Mem_CopyBytes_Arm7(req->pInfo, PARENT, sizeof(WMBssDesc));
    if (PARENT->gameInfoLength >= 16 && !(PARENT->gameInfo.attribute & WM_ATTR_FLAG_ENTRY)) {
        reply = Wmsp_GetCallbackBuffer();
        reply->apiid = WM_APIID_START_CONNECT;
        reply->errcode = WM_ERRCODE_NO_ENTRY;
        reply->state = WM_STATECODE_CONNECT_START;
        Wmsp_SendReply(reply);
        return;
    }
    channelBit = 1 << PARENT->channel;
    if (!(channelBit & buf->status->enableChannel) || !((channelBit >> 1) & WIRELESS_JOINABLE_CHANNELS)) {
        reply = Wmsp_GetCallbackBuffer();
        reply->apiid = WM_APIID_START_CONNECT;
        reply->errcode = WM_ERRCODE_INVALID_PARAM;
        reply->state = WM_STATECODE_CONNECT_START;
        Wmsp_SendReply(reply);
        return;
    }
    reply = Wmsp_GetCallbackBuffer();
    reply->apiid = WM_APIID_START_CONNECT;
    reply->errcode = WM_ERRCODE_SUCCESS;
    reply->state = WM_STATECODE_CONNECT_START;
    Wmsp_SendReply(reply);

    status = buf->status;
    if (status->unk_1e8 == 1) {
        if (PARENT->rateSetBasic & 1) {
            status->unk_1e8 = 1;
        } else {
            status->unk_1e8 = 2;
        }
    } else {
        if (PARENT->rateSetBasic & 2) {
            status->unk_1e8 = 2;
        } else {
            status->unk_1e8 = 1;
        }
    }
    if (PARENT->capaInfo & CAPA_SHORT_PREAMBLE) {
        buf->status->unk_1ea = 1;
    } else {
        buf->status->unk_1ea = 0;
    }
    if (PARENT->capaInfo & CAPA_PRIVACY) {
        buf->status->wepFlag = 1;
    } else {
        buf->status->wepFlag = 0;
    }
    if (PARENT->gameInfoLength == 0) {
        buf->status->unk_e2 = 3;
    } else {
        buf->status->unk_e2 = 2;
    }
    if (!WMSP_SetAllParams(WM_APIID_START_CONNECT, cmd)) {
        return;
    }
    wlReply = func_arm7_027c1ee4(cmd);
    if (wlReply[2] != 0) {
        WMSP_ReplyStartConnectFailed(WIFI_REQ_ENTER_READY, wlReply[2]);
        return;
    }
    buf->status->state = WM_STATE_CLASS1;
    powerSave = req->powerSave != 0;
    wlReply = func_arm7_027c2890(cmd, powerSave, 0, 1);
    if (wlReply[2] != 0) {
        WMSP_ReplyStartConnectFailed(WIFI_REQ_DOZE_SETTING, wlReply[2]);
        return;
    }
    buf->status->unk_c2 = powerSave;

    Mem_CopyBytes_Arm7(PARENT, &join, sizeof(join) - sizeof(join.unk_40));
    if (buf->status->unk_e2 == 2) {
        /* A DS parent's SSID: ggid, tgid, two zero bytes, then the 24 bytes
           the game passed. */
        join.ssidLength = 32;
        join.ssid[0] = PARENT->gameInfo.ggid;
        join.ssid[1] = PARENT->gameInfo.ggid >> 16;
        join.ssid[2] = PARENT->gameInfo.tgid;
        join.ssid[3] = 0;
        Mem_CopyBytes_Arm7(req->ssid, &join.ssid[4], sizeof(req->ssid));
    }
    wlReply = func_arm7_027c2750(cmd, WIFI_JOIN_STEP_WAIT_MS, &join);
    if (wlReply[2] != 0 || wlReply[3] != 0) {
        WMSP_ReplyStartConnectFailed(WIFI_REQ_SYNC_TO_PARENT, wlReply[2]);
        return;
    }
    Mem_CopyBytes_Arm7(&wlReply[4], buf->status->parentMacAddress, 6);
    buf->status->unk_18e = PARENT->gameInfo.childMaxSize + 2
                           + ((PARENT->gameInfo.attribute & WIRELESS_BEACON_BIT_KEYSHARE) ? 6 : 0);
    buf->status->unk_190 = PARENT->gameInfo.parentMaxSize + 4
                           + ((PARENT->gameInfo.attribute & WIRELESS_BEACON_BIT_KEYSHARE) ? 42 : 0);

    Mem_CopyBytes_Arm7(buf->status->parentMacAddress, authMac, 6);
    wlReply = func_arm7_027c26cc(cmd, authMac, req->authMode, WIFI_JOIN_STEP_WAIT_MS);
    if (wlReply[2] == WIFI_ANSWER_PEER_SAID_NO && wlReply[3] == WIFI_JOIN_REFUSED_FULL) {
        reply = Wmsp_GetCallbackBuffer();
        reply->apiid = WM_APIID_START_CONNECT;
        reply->errcode = WM_ERRCODE_OVER_MAX_ENTRY;
        reply->state = WM_STATECODE_CONNECT_START;
        Wmsp_SendReply(reply);
        return;
    }
    if (wlReply[2] != 0 || wlReply[3] != 0) {
        WMSP_ReplyStartConnectFailed(WIFI_REQ_AUTH, wlReply[2]);
        return;
    }

    Mem_CopyBytes_Arm7(buf->status->parentMacAddress, assocMac, 6);
    wlReply = func_arm7_027c25c8(cmd, assocMac, 1, WIFI_JOIN_STEP_WAIT_MS);
    intr = Cpu_MaskIrq_Arm7();
    if (wlReply[2] == WIFI_ANSWER_PEER_SAID_NO && wlReply[3] == WIFI_JOIN_REFUSED_FULL) {
        Cpu_RestoreIrqBit_Arm7(intr);
        reply = Wmsp_GetCallbackBuffer();
        reply->apiid = WM_APIID_START_CONNECT;
        reply->errcode = WM_ERRCODE_OVER_MAX_ENTRY;
        reply->state = WM_STATECODE_CONNECT_START;
        Wmsp_SendReply(reply);
        return;
    }
    if (wlReply[2] != 0 || wlReply[3] != 0) {
        Cpu_RestoreIrqBit_Arm7(intr);
        WMSP_ReplyStartConnectFailed(WIFI_REQ_ASSOC, wlReply[2]);
        return;
    }
    buf->status->aid = wlReply[4];
    buf->status->tgid = PARENT->gameInfo.tgid;
    one = 1;
    Mem_FillHalfwords_Arm7(one, buf->status->portSeqNo[0], sizeof(buf->status->portSeqNo[0]));
    rssi = Wmsp_DecodeRssi(PARENT->rssi & 0xff);
    buf->status->linkLevel = Wmsp_RssiToLinkLevel(rssi);
    Wmsp_ResetRssiHistory(rssi);

    intr2 = Cpu_MaskIrq_Arm7();
    buf->status->childBitmap = 1;
    buf->status->mpReadyBitmap = 1;
    if (buf->status->unk_7b0 != 0) {
        buf->status->mpLastRecvTick[0] = Tick_Read64_Arm7() | 1;
    }
    buf->status->state = WM_STATE_CHILD;
    Cpu_RestoreIrqBit_Arm7(intr2);
    buf->status->unk_be = 1;
    reply = Wmsp_GetCallbackBuffer();
    reply->apiid = WM_APIID_START_CONNECT;
    reply->errcode = WM_ERRCODE_SUCCESS;
    reply->state = WM_STATECODE_CONNECTED;
    reply->aid = buf->status->aid;
    Wmsp_SendReply(reply);
    Cpu_RestoreIrqBit_Arm7(intr);
}
