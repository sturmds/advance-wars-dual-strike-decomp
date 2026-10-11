#include "sdk/mi.h"

int OS_DisableInterrupts(void);
void OS_RestoreInterrupts(int irqState);

/* Stops a channel and clears its start timing and repeat bits. */
void MI_StopDma(u32 dmaNo)
{
    int irqState = OS_DisableInterrupts();
    volatile u16 *control = &IO_DMA_CTRL_UPPER(dmaNo);

    *control &= ~((DMA_START_MASK | DMA_REPEAT) >> 16);
    *control &= ~(DMA_ENABLE >> 16);
    (void)*control;
    (void)*control;
    if (dmaNo == 0) {
        DmaChannel *dma = &REG_DMA[dmaNo];

        dma->src = 0;
        dma->dest = 0;
        dma->control = DMA_DUMMY_TRANSFER;
    }
    OS_RestoreInterrupts(irqState);
}
