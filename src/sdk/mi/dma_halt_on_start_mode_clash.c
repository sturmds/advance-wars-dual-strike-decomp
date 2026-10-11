#include "sdk/mi.h"

void OS_Terminate(void);

/* Halts if another channel is waiting on a different start event than the one about to be set. */
void Dma_HaltOnStartModeClash(u32 dmaNo, u32 startMode)
{
    int i;

    for (i = 0; i < 3; i++) {
        u32 control;
        u32 timing;

        if (i == dmaNo) {
            continue;
        }
        control = REG_DMA[i].control;
        if (!(control & DMA_ENABLE)) {
            continue;
        }
        timing = control & DMA_START_MASK;
        if (timing == startMode) {
            continue;
        }
        if (timing == DMA_START_VBLANK && startMode == DMA_START_HBLANK) {
            continue;
        }
        if (timing == DMA_START_HBLANK && startMode == DMA_START_VBLANK) {
            continue;
        }
        if (timing == DMA_START_DISPLAY || timing == DMA_START_MAIN_MEMORY_DISPLAY
            || timing == DMA_START_DS_CARD || timing == DMA_START_GBA_CARD
            || timing == DMA_START_GEOMETRY_FIFO || timing == DMA_START_VBLANK
            || timing == DMA_START_HBLANK) {
            OS_Terminate();
        }
    }
}
