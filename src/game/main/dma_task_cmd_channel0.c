#include "types.h"
#include "game/task.h"
#include "game/dma_task.h"

void MI_StopDma(int channel);   /* SDK: stops a DMA channel */
void func_0210f3cc(int channel, const void *src, volatile void *dest, int mode);   /* SDK: starts a DMA transfer */


/* Script command of the channel 0 DMA task: restarts DMA channel 0 with the
 * task's source and destination, then calls the task's callback. The callback
 * is skipped when channel 1 is in use with the same one. */
void DmaTaskCmd_StartChannel0(Task *task)
{
    MI_StopDma(0);
    func_0210f3cc(0, DMA_TASK_ARGS(task)->src, DMA_TASK_ARGS(task)->dest, 2);
    if (gDmaTaskState.channelScreen[1] != 0 && DMA_TASK_ARGS(task)->callback == gDmaTaskState.channel1Callback) {
        return;
    }
    DMA_TASK_ARGS(task)->callback();
}
