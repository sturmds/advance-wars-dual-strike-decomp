#include "sdk/mi.h"

int OS_DisableInterrupts(void);
void OS_RestoreInterrupts(int irqState);

void Dma_LoadChannelMaskedSettle(u32 channel, u32 src, u32 dest, u32 control) {
  int irqState = OS_DisableInterrupts();
  DmaChannel* dma = &REG_DMA[channel];

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
  OS_RestoreInterrupts(irqState);
}
