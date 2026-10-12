#include "types.h"
#include "sdk/mi.h"

int func_arm7_037fdb80(void);
void func_arm7_037fdb94(int state);

/* The ARM7 MI_StopDma: stops a channel and clears its start timing and
 * repeat bits; channel 0 is then re-armed with a dummy transfer. */
void func_arm7_037fdd34(u32 dmaNo)
{
    int irqState = func_arm7_037fdb80();
    volatile u16 *control = &IO_DMA_CTRL_UPPER(dmaNo);

    *control &= ~((DMA7_START_MASK | DMA_REPEAT) >> 16);
    *control &= ~(DMA_ENABLE >> 16);
    (void)*control;
    (void)*control;
    if (dmaNo == 0) {
        DmaChannel *dma = &REG_DMA[dmaNo];

        dma->src = 0;
        dma->dest = 0;
        dma->control = DMA_DUMMY_TRANSFER;
    }
    func_arm7_037fdb94(irqState);
}
