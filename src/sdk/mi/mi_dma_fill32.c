#include "sdk/mi.h"
#include "sdk/os.h"

void Dma_LoadChannelAndSettle(u32 dmaNo, u32 src, u32 dest, u32 control);

/* Fills size bytes at dest with data in 32-bit units and waits until the transfer is done. */
void MI_DmaFill32(u32 dmaNo, u32 dest, u32 data, u32 size) {
  volatile u32* control;
  int irqState;

  if (size == 0) {
    return;
  }
  control = &((volatile u32*)REG_DMA)[dmaNo * 3 + 2];
  while (*control & DMA_ENABLE) {
  }
  irqState = OS_DisableInterrupts();
  IO_DMA_FILL_VALUE(dmaNo) = data;
  Dma_LoadChannelAndSettle(
      dmaNo, (u32)&IO_DMA_FILL_VALUE(dmaNo), dest,
      DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED | (size >> 2));
  OS_RestoreInterrupts(irqState);
  while (*control & DMA_ENABLE) {
  }
}
