#include "sdk/mi.h"

void MI_WaitDma(u32 dmaNo);
void MIi_CheckDma0SourceAddress(u32 dmaNo, u32 src, u32 size, u32 srcControl);
void Dma_LoadChannelMasked(u32 dmaNo, u32 src, u32 dest, u32 control);
void OSi_EnterDmaCallback(u32 dmaNo, void (*callback)(void*), void* arg);

/* Copies size bytes in 32-bit units. With a callback, the transfer raises an interrupt and callback(arg) runs after it. */
void MI_DmaCopy32Async(u32 dmaNo, u32 src, u32 dest, u32 size,
                       void (*callback)(void*), void* arg) {
  MIi_CheckDma0SourceAddress(dmaNo, src, size, 0);
  if (size == 0) {
    if (callback != NULL) {
      callback(arg);
    }
    return;
  }
  MI_WaitDma(dmaNo);
  if (callback != NULL) {
    OSi_EnterDmaCallback(dmaNo, callback, arg);
    Dma_LoadChannelMasked(dmaNo, src, dest,
                          DMA_ENABLE | DMA_IRQ | DMA_32BIT | (size >> 2));
  } else {
    Dma_LoadChannelMasked(dmaNo, src, dest,
                          DMA_ENABLE | DMA_32BIT | (size >> 2));
  }
}
