#include "sdk/mi.h"

void MIi_DmaSetParams(u32 channel, u32 src, u32 dest, u32 control)
{
    DmaChannel *dma = &REG_DMA[channel];

    dma->src = src;
    dma->dest = dest;
    dma->control = control;
}
