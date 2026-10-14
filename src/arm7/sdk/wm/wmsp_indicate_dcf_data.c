#include "types.h"
#include "sdk/wm.h"
#include "sdk/wmsp.h"
#include "sdk/mi.h"

u8 Wmsp_DecodeRssi(int rawRssi);
void Wmsp_PushRssiSample(u8 rssi);
int Wmsp_GetLinkLevel(void);
BOOL Wmsp_IsOwnMacAddress(const u8* macAddress);

/*
 * WIFI_HOSTMSG_DATA_FRAME: while DCF runs, updates the link level from the frame's RSSI and,
 * unless this station sent it, copies the frame into the next DCF receive buffer
 * and sends the ARM9 START_DCF/DCF_IND.
 */
void Wmsp_IndicateDcfData(WlDataIndication* ind) {
  WMStatus* status = func_arm7_027c05e0()->buf->status;
  WlDataFrame* frame;
  WlDataFrame* recvBuf;
  RadioDcfReply* callback;
  u32 size;

  if (status->unk_10 == 0) {
    return;
  }
  func_arm7_027c05e0(); /* needed to match: the original calls it and ignores the result */
  Wmsp_PushRssiSample(Wmsp_DecodeRssi(ind->frame.rssi));
  status->linkLevel = Wmsp_GetLinkLevel();
  frame = &ind->frame;
  if (Wmsp_IsOwnMacAddress(frame->srcAdrs) == TRUE) {
    return;
  }
  status->dcfRecvBufSel ^= 1;
  recvBuf = status->dcfRecvBuf[status->dcfRecvBufSel];
  size = frame->length + sizeof(WlDataFrame);
  Mem_CopyBytes_Arm7(frame, recvBuf, (size + 1) & ~1);
  Mem_CopyBytes_Arm7(frame->destAdrs, recvBuf->destAdrs, MAC_ADDRESS_SIZE);
  Mem_CopyBytes_Arm7(frame->srcAdrs, recvBuf->srcAdrs, MAC_ADDRESS_SIZE);

  callback = Wmsp_GetCallbackBuffer();
  callback->apiid = WM_APIID_START_DCF;
  callback->errcode = WM_ERRCODE_SUCCESS;
  callback->state = WIRELESS_NOTICE_DCF_FRAME;
  callback->recvBuf = recvBuf;
  Wmsp_SendReply(callback);
}
