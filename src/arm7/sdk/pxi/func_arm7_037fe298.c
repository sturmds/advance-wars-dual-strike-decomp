#include "types.h"
#include "sdk/pxi.h"
#include "sdk/os_arm7.h"

/* The ARM7 receive callback per tag. */
extern PXIFifoCallback data_arm7_03807ab8[FIFO_TAG_LIMIT];

/* The ARM7 PXI_SetFifoRecvCallback: registers callback for messages with
 * tag (NULL removes it) and publishes that to the ARM9. */
void func_arm7_037fe298(u32 tag, PXIFifoCallback callback)
{
    volatile FifoPeerRegistry *work;
    int irq = Cpu_MaskIrq_Arm7();

    data_arm7_03807ab8[tag] = callback;
    work = FIFO_PEER_REGISTRY;
    if (callback != NULL) {
        work->callbackFlags[PXI_PROC_ARM7] |= 1 << tag;
    } else {
        work->callbackFlags[PXI_PROC_ARM7] &= ~(1 << tag);
    }
    Cpu_RestoreIrqBit_Arm7(irq);
}
