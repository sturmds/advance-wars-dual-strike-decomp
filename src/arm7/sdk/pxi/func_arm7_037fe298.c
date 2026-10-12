#include "types.h"
#include "sdk/pxi.h"

/* The ARM7 receive callback per tag. */
extern PXIFifoCallback data_arm7_03807ab8[FIFO_TAG_LIMIT];

int func_arm7_037fdb80(void);
void func_arm7_037fdb94(int state);

/* The ARM7 PXI_SetFifoRecvCallback: registers callback for messages with
 * tag (NULL removes it) and publishes that to the ARM9. */
void func_arm7_037fe298(u32 tag, PXIFifoCallback callback)
{
    volatile FifoPeerRegistry *work;
    int irq = func_arm7_037fdb80();

    data_arm7_03807ab8[tag] = callback;
    work = FIFO_PEER_REGISTRY;
    if (callback != NULL) {
        work->callbackFlags[PXI_PROC_ARM7] |= 1 << tag;
    } else {
        work->callbackFlags[PXI_PROC_ARM7] &= ~(1 << tag);
    }
    func_arm7_037fdb94(irq);
}
