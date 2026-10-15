#include "sdk/mi.h"
#include "sdk/os.h"
#include "sdk/gx_load.h"

/* Needed to match: this function was compiled at -O2, and needs the src local. */
#pragma optimization_level 2

void MIi_DmaSetParams(u32 dmaNo, u32 src, u32 dest, u32 control);
void OSi_EnterDmaCallback(u32 dmaNo, void (*callback)(void*), void* arg);

/* Fills size bytes at dest with data in 32-bit units; with a callback, runs callback(arg) after the transfer. */
void MI_DmaFill32Async(u32 dmaNo, u32 dest, u32 data, u32 size,
                       void (*callback)(void*), void* arg) {
  int irqState;
  u32 src;

  if (size == 0) {
    if (callback != NULL) {
      callback(arg);
    }
    return;
  }
  MI_WaitDma(dmaNo);
  if (callback != NULL) {
    OSi_EnterDmaCallback(dmaNo, callback, arg);
    irqState = OS_DisableInterrupts();
    src = (u32)&IO_DMA_FILL_VALUE(dmaNo);
    IO_DMA_FILL_VALUE(dmaNo) = data;
    MIi_DmaSetParams(
        dmaNo, src, dest,
        DMA_ENABLE | DMA_IRQ | DMA_32BIT | DMA_SRC_FIXED | (size >> 2));
    OS_RestoreInterrupts(irqState);
  } else {
    irqState = OS_DisableInterrupts();
    src = (u32)&IO_DMA_FILL_VALUE(dmaNo);
    IO_DMA_FILL_VALUE(dmaNo) = data;
    MIi_DmaSetParams(dmaNo, src, dest,
                     DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED | (size >> 2));
    OS_RestoreInterrupts(irqState);
  }
}
