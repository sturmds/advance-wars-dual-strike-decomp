#include "sdk/snd.h"
#include "sdk/os.h"

/* With commands waiting, the tag to wait for is the newest flushed one; otherwise nothing is pending. */
u32 SND_GetCurrentCommandTag(void)
{
    int irq = OS_DisableInterrupts();
    u32 tag;

    if (data_022a9c70 == NULL) {
        tag = data_022a9c6c;
    } else {
        tag = data_022a9c88;
    }
    OS_RestoreInterrupts(irq);
    return tag;
}
