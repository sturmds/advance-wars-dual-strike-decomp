#include "types.h"
#include "game/dma_task.h"

void MI_StopDma(int channel);

void Dma_StopChannel0(void) {
  MI_StopDma(0);
  gDmaTaskState.channelScreen[0] = 0;
}
