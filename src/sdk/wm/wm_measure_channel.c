#include "types.h"
#include "sdk/wm.h"

/* Listens on one channel and reports how busy it is. */
int WM_MeasureChannel(WMCallbackFunc callback, u16 ccaMode, u16 edThreshold,
                      u16 channel, u16 measureTime) {
  WMArm9Buf* buf = func_02116e10();
  WMMeasureChannelReq* req;
  int result = WMi_CheckStateEx(1, WM_STATE_IDLE);

  if (result != 0) {
    return result;
  }
  func_02116f74(WM_APIID_MEASURE_CHANNEL, callback);
  req = buf->requestBuf;
  req->apiid = WM_APIID_MEASURE_CHANNEL;
  req->ccaMode = ccaMode;
  req->edThreshold = edThreshold;
  req->channel = channel;
  req->measureTime = measureTime;
  result = WMi_SendCommand(WM_APIID_MEASURE_CHANNEL, 0);
  if (result == 0) {
    result = WM_ERRCODE_OPERATING;
  }
  return result;
}
