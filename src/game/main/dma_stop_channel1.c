#include "types.h"
#include "game/dma_task.h"

void MI_StopDma(int channel); /* SDK: stops a DMA channel */

/* Stops DMA channel 1 and clears its flag. */
void Dma_StopChannel1(void) {
  MI_StopDma(1);
  gDmaTaskState.channelScreen[1] = 0;
}
