#include "sdk/mi.h"

void Dma_HaltOnStartModeClash(u32 dmaNo, u32 startMode);
void MIi_CheckDma0SourceAddress(u32 dmaNo, u32 src, u32 size, u32 srcControl);
void Dma_LoadChannelMasked(u32 dmaNo, u32 src, u32 dest, u32 control);

/* Starts a 32-bit copy triggered by the cartridge slot, with a fixed source, and returns without waiting. */
void MIi_CardDmaCopy32(u32 dmaNo, u32 src, u32 dest, u32 size)
{
    volatile u32 *control;

    Dma_HaltOnStartModeClash(dmaNo, 0xFFFFFFFF);
    MIi_CheckDma0SourceAddress(dmaNo, src, size, DMA_SRC_FIXED);
    if (size == 0) {
        return;
    }
    control = &((volatile u32 *)REG_DMA)[dmaNo * 3 + 2];
    while (*control & DMA_ENABLE) {
    }
    /* The count field is 1: the card controller triggers one word at a time. */
    Dma_LoadChannelMasked(dmaNo, src, dest,
                          DMA_ENABLE | DMA_START_DS_CARD | DMA_REPEAT | DMA_32BIT | DMA_SRC_FIXED | 1);
}
