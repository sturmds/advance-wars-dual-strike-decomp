#include "types.h"
#include "game/task.h"
#include "game/dma_task.h"

void MI_StopDma(int channel);   /* SDK: stops a DMA channel */
void func_0210f3cc(int channel, const void *src, volatile void *dest, int mode);   /* SDK: starts a DMA transfer */

/* Script command of the channel 1 DMA task: restarts DMA channel 1 with the
 * task's source and destination, then calls the task's callback. */
void DmaTaskCmd_StartChannel1(Task *task)
{
    MI_StopDma(1);
    func_0210f3cc(1, DMA_TASK_ARGS(task)->src, DMA_TASK_ARGS(task)->dest, 2);
    DMA_TASK_ARGS(task)->callback();
}
