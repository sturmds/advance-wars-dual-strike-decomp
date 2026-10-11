#include "types.h"
#include "game/dma_task.h"

void func_0210efb0(int channel);

void Dma_StopChannel0(void)
{
    func_0210efb0(0);
    gDmaTaskState.channelScreen[0] = 0;
}
