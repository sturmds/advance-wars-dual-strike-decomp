#include "types.h"
#include "sdk/wm.h"

int func_02116f74(int apiid, WMCallbackFunc callback) {
  WMArm9Buf* buf = data_022ac32c;
  buf->callbackTable[apiid] = callback;
  return (int)buf + (apiid << 2);
}
