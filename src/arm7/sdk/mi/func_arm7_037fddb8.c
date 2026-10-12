#include "types.h"
#include "sdk/mi.h"

int func_arm7_037fdb80(void);
void func_arm7_037fdb94(int state);

/* The ARM7 MI_WaitDma: waits for a channel to finish, then re-arms
 * channel 0 with a dummy transfer. */
void func_arm7_037fddb8(u32 dmaNo)
{
    int irqState = func_arm7_037fdb80();
    volatile u32 *control = &((volatile u32 *)REG_DMA)[dmaNo * 3 + 2];

    while (*control & DMA_ENABLE) {
    }
    if (dmaNo == 0) {
        DmaChannel *dma = &REG_DMA[dmaNo];

        dma->src = 0;
        dma->dest = 0;
        dma->control = DMA_DUMMY_TRANSFER;
    }
    func_arm7_037fdb94(irqState);
}
