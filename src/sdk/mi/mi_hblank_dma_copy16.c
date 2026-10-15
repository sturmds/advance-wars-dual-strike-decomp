#include "sdk/mi.h"
#include "sdk/gx_load.h"

/* Destination control 3: increment and reload the address on each repeat. */
#define DMA_DEST_RELOAD 0x00600000

void Dma_HaltOnStartModeClash(u32 dmaNo, u32 startMode);
void MIi_CheckDma0SourceAddress(u32 dmaNo, u32 src, u32 size, u32 srcControl);
void Dma_LoadChannelMasked(u32 dmaNo, u32 src, u32 dest, u32 control);

/* Starts a repeating 16-bit copy that runs once per HBlank, and returns without waiting. */
void MI_HBlankDmaCopy16(u32 dmaNo, u32 src, u32 dest, u32 size) {
  Dma_HaltOnStartModeClash(dmaNo, DMA_START_HBLANK);
  MIi_CheckDma0SourceAddress(dmaNo, src, size, 0);
  if (size == 0) {
    return;
  }
  MI_WaitDma(dmaNo);
  Dma_LoadChannelMasked(dmaNo, src, dest,
                        DMA_ENABLE | DMA_START_HBLANK | DMA_REPEAT |
                            DMA_DEST_RELOAD | (size >> 1));
}
