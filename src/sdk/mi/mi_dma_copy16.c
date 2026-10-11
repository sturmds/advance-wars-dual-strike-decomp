#include "sdk/mi.h"

void MIi_CheckDma0SourceAddress(u32 dmaNo, u32 src, u32 size, u32 srcControl);
void Dma_LoadChannelMaskedSettle(u32 dmaNo, u32 src, u32 dest, u32 control);

/* Copies size bytes in 16-bit units and waits until the transfer is done. */
void MI_DmaCopy16(u32 dmaNo, u32 src, u32 dest, u32 size)
{
    volatile u32 *control;

    if (size == 0) {
        return;
    }
    MIi_CheckDma0SourceAddress(dmaNo, src, size, 0);
    control = &((volatile u32 *)REG_DMA)[dmaNo * 3 + 2];
    while (*control & DMA_ENABLE) {
    }
    Dma_LoadChannelMaskedSettle(dmaNo, src, dest, DMA_ENABLE | (size >> 1));
    while (*control & DMA_ENABLE) {
    }
}
