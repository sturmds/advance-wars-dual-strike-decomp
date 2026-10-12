#include "sdk/wmsp.h"

/* WL command ids, named after what the handler does around them. */
#define WIFI_REQ_FIND_PARENTS 2
#define WIFI_REQ_ENTER_READY \
  0x303 /* WMStatus.state becomes WM_STATE_CLASS1 after it */
#define WIFI_REQ_READ_MODE 0x308 /* reply word 3: the WL state */

#define WIFI_REPORTED_RADIO_ON \
  16 /* the WL needs setup and class 1 before scanning */

u16* func_arm7_027c1e74(WlCommand* cmd);
u16* func_arm7_027c1ee4(WlCommand* cmd);
u16* func_arm7_027c2890(WlCommand* cmd, int arg0, int arg1, int arg2);
u16* func_arm7_027c27d4(WlCommand* cmd, u32 size, const void* mac, int arg0,
                        const void* block32, u16 arg1, const void* block16,
                        u16 arg2);
u8 Wmsp_DecodeRssi(int rawRssi);
int Wmsp_RssiToLinkLevel(u32 rssi);

/* Allowed in WM_STATE_IDLE, WM_STATE_CLASS1 and WM_STATE_SCAN: scans one
   channel for a parent and reports the first one found. Its beacon goes to
   the request's scanBuf, and a copy of the main fields to the reply. */
void WMSP_StartScan(WMStartScanReq* req) {
  WlCommandBuf cmdBuf;
  WlCommand* cmd = &cmdBuf.command;
  WmspBuf* buf = func_arm7_027c05e0()->buf;
  u16 bssid[3];
  u16 ssid[16];
  u8 channels[16];
  volatile u16 ssidFill;
  volatile u16 zeroInfo;
  volatile u16 zeroGameInfo;
  WMStartScanCallback* reply;
  u16* result;
  WmspScanReply* scan;
  u16 state;
  u16 channel;
  u16 maxChannelTime;

  state = buf->status->state;
  if (state != WM_STATE_IDLE && state != WM_STATE_CLASS1 &&
      state != WM_STATE_SCAN) {
    reply = Wmsp_GetCallbackBuffer();
    reply->apiid = WM_APIID_START_SCAN;
    reply->errcode = WM_ERRCODE_ILLEGAL_STATE;
    reply->state = WM_STATECODE_PARENT_NOT_FOUND;
    Wmsp_SendReply(reply);
    return;
  }
  buf->status->pInfoBuf = req->scanBuf;
  channel = req->channel;
  buf->status->scanChannel = channel;
  maxChannelTime = req->maxChannelTime;
  Mem_CopyBytes_Arm7(req->bssid, bssid, sizeof(bssid));
  /* all 0xff scans for any parent; a scan for one clears the group bit */
  if (bssid[0] != 0xffff && (bssid[0] & 1)) {
    bssid[0] &= ~1;
  }
  if (channel == 0) {
    reply = Wmsp_GetCallbackBuffer();
    reply->apiid = WM_APIID_START_SCAN;
    reply->errcode = WM_ERRCODE_INVALID_PARAM;
    reply->state = WM_STATECODE_PARENT_NOT_FOUND;
    Wmsp_SendReply(reply);
    return;
  }
  if (((1 << channel) & buf->status->enableChannel) == 0) {
    reply = Wmsp_GetCallbackBuffer();
    reply->apiid = WM_APIID_START_SCAN;
    reply->errcode = WM_ERRCODE_INVALID_PARAM;
    reply->state = WM_STATECODE_PARENT_NOT_FOUND;
    Wmsp_SendReply(reply);
    return;
  }
  buf->status->unk_e2 = 2;
  result = func_arm7_027c1e74(cmd);
  if (result[2] != 0) {
    WMSP_ReplyStartScanFailed(WIFI_REQ_READ_MODE, result[2], FALSE);
    return;
  }
  if (result[3] == WIFI_REPORTED_RADIO_ON) {
    if (!WMSP_SetAllParams(WM_APIID_START_SCAN, cmd)) {
      return;
    }
    result = func_arm7_027c1ee4(cmd);
    if (result[2] != 0) {
      WMSP_ReplyStartScanFailed(WIFI_REQ_ENTER_READY, result[2], FALSE);
      return;
    }
    buf->status->state = WM_STATE_CLASS1;
    result = func_arm7_027c2890(cmd, 1, 0, 1);
    if (result[2] != 0) {
      WMSP_ReplyStartScanFailed(1, result[2], FALSE);
      return;
    }
    buf->status->unk_c2 = 1;
  }
  buf->status->state = WM_STATE_SCAN;
  ssidFill = 0xffff;
  Mem_FillHalfwords_Arm7(ssidFill, ssid, sizeof(ssid));
  channels[0] = channel;
  Mem_FillBytes_Arm7(&channels[1], 0, 15);
  /* WlScanRequest: SSID length 0, and 1 sends no probe requests */
  scan = (WmspScanReply*)func_arm7_027c27d4(cmd, 0x11c, bssid, 0, ssid, 1,
                                            channels, maxChannelTime);
  if (scan->status != 0) {
    WMSP_ReplyStartScanFailed(WIFI_REQ_FIND_PARENTS, scan->status, FALSE);
    return;
  }
  reply = Wmsp_GetCallbackBuffer();
  if (scan->bssCount == 0) {
    reply->apiid = WM_APIID_START_SCAN;
    reply->errcode = WM_ERRCODE_SUCCESS;
    reply->state = WM_STATECODE_PARENT_NOT_FOUND;
    reply->channel = channel;
    reply->linkLevel = 0;
  } else {
    zeroInfo = 0;
    Mem_FillHalfwords_Arm7(zeroInfo, &buf->status->pInfoBuf->gameInfo,
                           sizeof(WMGameInfo));
    Mem_CopyBytes_Arm7(&scan->bssLength, buf->status->pInfoBuf,
                       scan->bssLength * 2);
    reply->apiid = WM_APIID_START_SCAN;
    reply->errcode = WM_ERRCODE_SUCCESS;
    reply->state = WM_STATECODE_PARENT_FOUND;
    reply->channel = scan->channel;
    reply->linkLevel = Wmsp_RssiToLinkLevel(Wmsp_DecodeRssi((u8)scan->rssi));
    reply->ssidLength = scan->ssidLength;
    Mem_CopyBytes_Arm7(scan->bssid, reply->macAddress,
                       sizeof(reply->macAddress));
    Mem_CopyHalfwords_Arm7(scan->ssid, reply->ssid, sizeof(reply->ssid));
    reply->gameInfoLength = scan->gameInfoLength;
    if (reply->gameInfoLength > sizeof(WMGameInfo)) {
      reply->apiid = WM_APIID_START_SCAN;
      reply->errcode = WM_ERRCODE_SUCCESS;
      reply->state = WM_STATECODE_PARENT_NOT_FOUND;
      reply->channel = channel;
      reply->linkLevel = 0;
    } else {
      zeroGameInfo = 0;
      Mem_FillHalfwords_Arm7(zeroGameInfo, &reply->gameInfo,
                             sizeof(WMGameInfo));
      Mem_CopyHalfwords_Arm7(scan->gameInfo, &reply->gameInfo,
                             (reply->gameInfoLength + 1) & ~1);
    }
  }
  Wmsp_SendReply(reply);
}
