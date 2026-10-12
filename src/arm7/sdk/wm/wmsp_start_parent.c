#include "sdk/os_arm7.h"
#include "sdk/wmsp.h"

u16* func_arm7_027c1ee4(WlCommand* cmd);
u16* func_arm7_027c2890(WlCommand* cmd, int arg0, int arg1, int arg2);
u16* func_arm7_027c24f4(WlCommand* cmd, u16 arg0, const void* block, u16 arg17,
                        u16 arg18, u16 arg19, u16 arg20, u16 arg21, u16 size,
                        const void* data);
void Wmsp_BuildGameInfo(WMGameInfo* info, const WMParentParam* param);
void WMSP_ReplyStartParentFailed(u16 wlCmdID, u16 wlResult);

/* As in sdk/mi.h, which clashes with wmsp.h over Mem_CopyHalfwords_Arm7. */

/* Only allowed in WM_STATE_IDLE: starts a parent with the parameters set by
   WMSP_SetParentParam. The SSID holds the ggid and tgid, the beacon the game
   info; then WM moves to WM_STATE_PARENT. */
void WMSP_StartParent(void) {
  WmspBuf* buf;
  WlCommandBuf cmdBuf;
  WlCommand* cmd = &cmdBuf.command;
  WMGameInfo gameInfo;
  u16 ssid[16];
  volatile u16 zeroInfo;
  volatile u16 zeroSsid;
  WMStatus* status;
  WMParentParam* param;
  WMStartParentCallback* reply;
  u16* result;
  int irq;

  buf = func_arm7_027c05e0()->buf;
  status = buf->status;
  if (status->state != WM_STATE_IDLE) {
    reply = Wmsp_GetCallbackBuffer();
    reply->apiid = WM_APIID_START_PARENT;
    reply->errcode = WM_ERRCODE_ILLEGAL_STATE;
    reply->state = WM_STATECODE_PARENT_START;
    Wmsp_SendReply(reply);
    return;
  }
  if ((status->allowedChannel &
       ((1 << Wmsp_ParentParam(status)->channel) >> 1)) == 0) {
    reply = Wmsp_GetCallbackBuffer();
    reply->apiid = WM_APIID_START_PARENT;
    reply->errcode = WM_ERRCODE_INVALID_PARAM;
    reply->state = WM_STATECODE_PARENT_START;
    Wmsp_SendReply(reply);
    return;
  }
  status->unk_e2 = 1;
  buf->status->aid = 0;
  irq = Cpu_MaskIrq_Arm7();
  buf->status->childBitmap = 0;
  buf->status->mpReadyBitmap = 0;
  Cpu_RestoreIrqBit_Arm7(irq);
  if (!WMSP_SetAllParams(WM_APIID_START_PARENT, cmd)) {
    return;
  }
  result = func_arm7_027c1ee4(cmd);
  if (result[2] != 0) {
    WMSP_ReplyStartParentFailed(0x303, result[2]);
    return;
  }
  result = func_arm7_027c2890(cmd, 1, 0, 1);
  if (result[2] != 0) {
    WMSP_ReplyStartParentFailed(1, result[2]);
    return;
  }
  buf->status->unk_c2 = 1;
  param = Wmsp_ParentParam(buf->status);
  zeroInfo = 0;
  Mem_FillHalfwords_Arm7(zeroInfo, &gameInfo, sizeof(gameInfo));
  Wmsp_BuildGameInfo(&gameInfo, param);
  zeroSsid = 0;
  Mem_FillHalfwords_Arm7(zeroSsid, ssid, sizeof(ssid));
  ssid[0] = param->ggid;
  ssid[1] = param->ggid >> 16;
  ssid[2] = param->tgid;
  ssid[3] = 0;
  result = func_arm7_027c24f4(cmd, sizeof(ssid), ssid, param->beaconPeriod, 2,
                              param->channel, 3, 3,
                              (u16)(param->userGameInfoLength + 16), &gameInfo);
  if (result[2] != 0) {
    WMSP_ReplyStartParentFailed(9, result[2]);
    return;
  }
  /* key sharing adds 42 bytes to the parent's MP frame and 6 to a child's */
  buf->status->unk_18e = param->parentMaxSize + 4 + (param->KS_Flag ? 42 : 0);
  buf->status->unk_190 = param->childMaxSize + 2 + (param->KS_Flag ? 6 : 0);
  reply = Wmsp_GetCallbackBuffer();
  buf->status->state = WM_STATE_PARENT;
  reply->apiid = WM_APIID_START_PARENT;
  reply->errcode = WM_ERRCODE_SUCCESS;
  reply->state = WM_STATECODE_PARENT_START;
  Wmsp_SendReply(reply);
  buf->status->unk_be = 1;
}
