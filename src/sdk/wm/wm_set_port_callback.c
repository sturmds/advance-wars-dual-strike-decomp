#include "types.h"
#include "sdk/os.h"
#include "sdk/wm.h"

/* Registers the function that receives data arriving on a port, with its
 * argument. Returns 0, or the error code when the WM is not initialised. */
int WM_SetPortCallback(int port, WMCallbackFunc callback, void* arg) {
  OSIntrMode enabled = OS_DisableInterrupts();
  int result = func_02116df4();
  WMArm9Buf* buf;

  if (result != 0) {
    OS_RestoreInterrupts(enabled);
    return result;
  }
  buf = func_02116e10();
  buf->portCallback[port] = callback;
  buf->portCallbackArg[port] = arg;
  OS_RestoreInterrupts(enabled);
  return 0;
}
