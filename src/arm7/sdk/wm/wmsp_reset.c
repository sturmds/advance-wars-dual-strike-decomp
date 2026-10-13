#include "sdk/wmsp.h"
#include "sdk/os_arm7.h"
#include "sdk/mi.h"

/* WL command ids, named after what the handler does around them. */
#define WIFI_REQ_ENTER_RADIO_ON \
  0x302 /* WMStatus.state becomes WM_STATE_IDLE after it */
#define WIFI_REQ_READ_MODE 0x308 /* reply word 3: the WL state */
#define WIFI_REQ_READ_ROLE \
  0x284 /* reply word 3: 1 on a parent, 2 or 3 on a child */

#define DEAUTH_REASON_LEAVING \
  3 /* IEEE 802.11 reason code: the station leaves */

void Wmsp_SetNormalThreadPriorities(void);
u16* func_arm7_027c1e74(WlCommand* cmd);
u16* func_arm7_027c1f2c(WlCommand* cmd);
u16* func_arm7_027c1efc(WlCommand* cmd);
u16* func_arm7_027c1e00(WlCommand* cmd, u16 arg0, u16 arg1, u16 arg2, u16 arg3);
u16* func_arm7_027c2648(WlCommand* cmd, const void* macAddress, int reason);
u16* func_arm7_027c28f4(WlCommand* cmd, int arg);
void WMSP_ReplyResetFailed(u16 wlCmdID, u16 wlResult);

/* Stops MP, then takes the WL from whatever state it is in back to idle:
   a connected station first deauthenticates (a child from its parent, a
   parent from every child). Leaves WM in WM_STATE_IDLE. */
void WMSP_Reset(void) {
  WmspBuf* buf = func_arm7_027c05e0()->buf;
  WlCommandBuf wlBuf;
  WlCommand* cmd = &wlBuf.command;
  u8 parentMac[6];
  u8 broadcastMac[6];
  BOOL mpWasOn = FALSE;
  int intr;
  u16* wlReply;
  u16 wlState;
  u16 wlMode;
  WmspReply* reply;

  intr = Cpu_MaskIrq_Arm7();
  if (buf->status->mpFlag == 1) {
    buf->status->mpFlag = 0;
    mpWasOn = TRUE;
    LineAlarm_Withdraw_Arm7(&buf->vAlarm);
    Wmsp_SetNormalThreadPriorities();
    if (buf->status->state == WM_STATE_MP_CHILD) {
      buf->status->state = WM_STATE_CHILD;
    } else if (buf->status->state == WM_STATE_MP_PARENT) {
      buf->status->state = WM_STATE_PARENT;
    }
  }
  buf->status->childBitmap = 0;
  buf->status->mpReadyBitmap = 0;
  buf->status->unk_14 = 0;
  buf->status->unk_10 = 0;
  buf->status->unk_1c = 0;
  buf->status->unk_be = 0;
  Cpu_RestoreIrqBit_Arm7(intr);
  if (mpWasOn) {
    SendQueue_DropChildren(0xffff);
  }

  wlReply = func_arm7_027c1e74(cmd);
  if (wlReply[2] != 0) {
    WMSP_ReplyResetFailed(WIFI_REQ_READ_MODE, wlReply[2]);
    return;
  }
  wlState = wlReply[3];
  wlReply = func_arm7_027c1f2c(cmd);
  if (wlReply[2] != 0) {
    WMSP_ReplyResetFailed(WIFI_REQ_READ_ROLE, wlReply[2]);
    return;
  }
  wlMode = wlReply[3];

  /* Each WL state undoes its own step, then falls into the steps below it. */
  switch (wlState) {
    case 0x30:
    case 0x40:
      /* needed to match: the original adds 0xfffe, not subtracts 2 */
      if ((u16)(wlMode + (u16)-2) <= 1) {
        Mem_CopyBytes_Arm7(buf->status->parentMacAddress, parentMac, 6);
        wlReply = func_arm7_027c2648(cmd, parentMac, DEAUTH_REASON_LEAVING);
        if (wlReply[2] == 0) {
          buf->status->state = WM_STATE_CLASS1;
        }
      } else if (wlMode == 1) {
        Mem_FillBytes_Arm7(broadcastMac, 0xff, 6);
        wlReply = func_arm7_027c2648(cmd, broadcastMac, DEAUTH_REASON_LEAVING);
        if (wlReply[2] == 0) {
          buf->status->state = WM_STATE_CLASS1;
        }
      }
      /* fall through */
    case 0x20:
      wlReply = func_arm7_027c28f4(cmd, 1);
      if (wlReply[2] != 0) {
        WMSP_ReplyResetFailed(0, wlReply[2]);
        return;
      }
      /* fall through */
    case 0x00:
      wlReply = func_arm7_027c1efc(cmd);
      if (wlReply[2] != 0) {
        WMSP_ReplyResetFailed(WIFI_REQ_ENTER_RADIO_ON, wlReply[2]);
        return;
      }
      /* fall through */
    case 0x10:
      buf->status->state = WM_STATE_IDLE;
      break;
    case 0x11:
    case 0x12:
      if (wlMode == 0) {
        wlReply = func_arm7_027c1e00(cmd, 0, 0, 20, 1);
        if (wlReply[2] != 0) {
          WMSP_ReplyResetFailed(0x309, wlReply[2]);
          return;
        }
      }
      wlReply = func_arm7_027c1efc(cmd);
      if (wlReply[2] != 0) {
        WMSP_ReplyResetFailed(WIFI_REQ_ENTER_RADIO_ON, wlReply[2]);
        return;
      }
      buf->status->state = WM_STATE_IDLE;
      break;
    default:
      WMSP_ReplyResetFailed(WIFI_REQ_READ_MODE, 0);
      return;
  }
  reply = Wmsp_GetCallbackBuffer();
  reply->apiid = WM_APIID_RESET;
  reply->errcode = WM_ERRCODE_SUCCESS;
  Wmsp_SendReply(reply);
}
