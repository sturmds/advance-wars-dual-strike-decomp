#include "types.h"
#include "sdk/os.h"
#include "sdk/pxi.h"

extern PXIFifoCallback gPxiRecvCallbacks[FIFO_TAG_LIMIT];

/* Registers callback for messages with tag (NULL removes it) and publishes
 * that to the other CPU. */
void PXI_SetFifoRecvCallback(u32 tag, PXIFifoCallback callback)
{
    volatile FifoPeerRegistry *work;
    int irq = OS_DisableInterrupts();

    gPxiRecvCallbacks[tag] = callback;
    work = FIFO_PEER_REGISTRY;
    if (callback != NULL) {
        work->callbackFlags[PXI_PROC_ARM9] |= 1 << tag;
    } else {
        work->callbackFlags[PXI_PROC_ARM9] &= ~(1 << tag);
    }
    OS_RestoreInterrupts(irq);
}
