#include "types.h"
#include "sdk/wm.h"
#include "sdk/os.h"

/* Starts the MP exchange (the parent polls its children every frame) with the
 * given receive and send buffers. Both must be at least the size WM asks for:
 * the receive buffer a multiple of 64 bytes, the send buffer of 32. callback
 * gets the MP events. Returns WM_ERRCODE_OPERATING when the command is
 * queued. */
int WM_StartMPEx(WMCallbackFunc callback, u16* recvBuf, u16 recvBufSize,
                 u16* sendBuf, u16 sendBufSize, u16 mpFreq,
                 u16 defaultRetryCount, BOOL minPollBmpMode,
                 BOOL singlePacketMode, BOOL fixFreqMode,
                 BOOL ignoreFatalError) {
  WMArm9Buf* buf = func_02116e10();
  int result;
  int sendSize;

  result = WMi_CheckStateEx(2, WM_STATE_PARENT, WM_STATE_CHILD);
  if (result != WM_ERRCODE_SUCCESS) {
    return result;
  }
  DC_InvalidateRange(&buf->status->mpFlag, 4);
  if (buf->status->mpFlag == 1) {
    return WM_ERRCODE_ILLEGAL_STATE;
  }
  if (recvBufSize < WM_GetMPReceiveBufferSize()) {
    return WM_ERRCODE_INVALID_PARAM;
  }
  if ((recvBufSize & 0x3f) != 0) {
    return WM_ERRCODE_INVALID_PARAM;
  }
  /* needed to match: the checks use an int copy, the command reloads
     * sendBufSize from the stack */
  sendSize = sendBufSize;
  if (sendSize < WM_GetMPSendBufferSize()) {
    return WM_ERRCODE_INVALID_PARAM;
  }
  if ((sendSize & 0x1f) != 0) {
    return WM_ERRCODE_INVALID_PARAM;
  }
  func_02116f74(WM_APIID_START_MP, callback);
  /* the ARM7 takes the receive buffer size in halfwords */
  result =
      WMi_SendCommand(WM_APIID_START_MP, 10, recvBuf, recvBufSize / 2, sendBuf,
                      sendBufSize, mpFreq, defaultRetryCount, minPollBmpMode,
                      singlePacketMode, fixFreqMode, ignoreFatalError);
  if (result == WM_ERRCODE_SUCCESS) {
    result = WM_ERRCODE_OPERATING;
  }
  return result;
}
