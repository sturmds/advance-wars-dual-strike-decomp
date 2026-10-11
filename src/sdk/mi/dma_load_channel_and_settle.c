#include "sdk/mi.h"

/*
 * Sets a channel's registers (the control write starts it), then reads the
 * channel 0 source register twice. Channel 0 is then loaded with a dummy
 * transfer.
 */
void Dma_LoadChannelAndSettle(u32 channel, u32 src, u32 dest, u32 control)
{
    DmaChannel *dma = &REG_DMA[channel];

    dma->src = src;
    dma->dest = dest;
    dma->control = control;
    (void)REG_DMA[0].src;
    (void)REG_DMA[0].src;
    if (channel == 0) {
        dma->src = 0;
        dma->dest = 0;
        dma->control = DMA_DUMMY_TRANSFER;
    }
    (void)REG_DMA[0].src;
    (void)REG_DMA[0].src;
}
