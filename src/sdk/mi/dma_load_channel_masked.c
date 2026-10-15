#include "sdk/mi.h"
#include "sdk/os.h"

void Dma_LoadChannelMasked(u32 channel, u32 src, u32 dest, u32 control) {
  int irqState = OS_DisableInterrupts();
  DmaChannel* dma = &REG_DMA[channel];

  dma->src = src;
  dma->dest = dest;
  dma->control = control;
  OS_RestoreInterrupts(irqState);
}
