#include "types.h"
#include "sdk/os.h"

extern OSThreadInfo OSi_ThreadInfo;

ThreadSwapHook OS_SetSwitchThreadCallback(ThreadSwapHook callback) {
  int irqState = OS_DisableInterrupts();
  ThreadSwapHook previous = OSi_ThreadInfo.switchCallback;

  OSi_ThreadInfo.switchCallback = callback;
  OS_RestoreInterrupts(irqState);
  return previous;
}
