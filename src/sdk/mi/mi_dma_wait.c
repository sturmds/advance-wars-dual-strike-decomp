#include "sdk/mi.h"

int OS_DisableInterrupts(void);
void OS_RestoreInterrupts(int irqState);

/* Waits for a channel to finish, then re-arms channel 0 with a dummy transfer. */
void MI_WaitDma(u32 dmaNo) {
  int irqState = OS_DisableInterrupts();
  volatile u32* control = &((volatile u32*)REG_DMA)[dmaNo * 3 + 2];

  while (*control & DMA_ENABLE) {
  }
  if (dmaNo == 0) {
    DmaChannel* dma = &REG_DMA[dmaNo];

    dma->src = 0;
    dma->dest = 0;
    dma->control = DMA_DUMMY_TRANSFER;
  }
  OS_RestoreInterrupts(irqState);
}
