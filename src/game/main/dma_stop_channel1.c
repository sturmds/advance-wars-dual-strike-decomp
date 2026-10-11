#include "types.h"
#include "game/dma_task.h"

void func_0210efb0(int channel);   /* SDK: stops a DMA channel */

/* Stops DMA channel 1 and clears its flag. */
void Dma_StopChannel1(void)
{
    func_0210efb0(1);
    gDmaTaskState.channelScreen[1] = 0;
}
