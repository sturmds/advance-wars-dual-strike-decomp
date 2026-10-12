#include "types.h"
#include "sdk/os.h"
#include "sdk/wm.h"

/* Registers the function that receives indications such as a child connecting
 * or a parent beacon being lost. Returns 0, or the error code when the WM is
 * not initialised. */
int WM_SetIndCallback(WMCallbackFunc callback) {
  OSIntrMode enabled = OS_DisableInterrupts();
  int result = func_02116df4();

  if (result != 0) {
    OS_RestoreInterrupts(enabled);
    return result;
  }
  func_02116e10()->indCallback = callback;
  OS_RestoreInterrupts(enabled);
  return 0;
}
