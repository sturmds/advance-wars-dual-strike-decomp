#include "types.h"
#include "sdk/os.h"

extern OSThreadInfo data_022a9930;

ThreadSwapHook OS_SetSwitchThreadCallback(ThreadSwapHook callback)
{
    int irqState = OS_DisableInterrupts();
    ThreadSwapHook previous = data_022a9930.switchCallback;

    data_022a9930.switchCallback = callback;
    OS_RestoreInterrupts(irqState);
    return previous;
}
