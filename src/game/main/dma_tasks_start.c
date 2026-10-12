#include "types.h"
#include "game/task.h"
#include "game/dma_task.h"

extern u32 gTaskSystemReady;
extern const TaskScriptCmd data_021571ac[]; /* DMA task on channel 0 */
extern const TaskScriptCmd data_021571cc[]; /* DMA task on channel 1 */

/* Starts one DMA task on each channel: src1 feeds dest1 on channel 0 and
 * src2 feeds dest2 on channel 1. Does nothing when either channel is in use. */
void DmaTasks_Start(const void* src1, const void* src2, volatile void* dest1,
                    volatile void* dest2, u16 unk34, u16 unk36,
                    void (*callback1)(void), void (*callback2)(void)) {
  Task* task;

  if (gDmaTaskState.channelScreen[0] != 0) {
    return;
  }
  if (gDmaTaskState.channelScreen[1] != 0) {
    return;
  }
  gDmaTaskState.channelScreen[0] = 1;
  gDmaTaskState.channelScreen[1] = 2;
  gDmaTaskState.channel1Callback = callback1;
  gDmaTaskState.pair = 1;
  gTaskSystemReady = 0;
  task = Task_Create(data_021571ac, TASK_LIST(0));
  DMA_TASK_ARGS(task)->src = src1;
  DMA_TASK_ARGS(task)->dest = dest1;
  DMA_TASK_ARGS(task)->unk_34 = unk34;
  DMA_TASK_ARGS(task)->unk_36 = unk36;
  DMA_TASK_ARGS(task)->callback = callback1;
  task = Task_Create(data_021571cc, TASK_LIST(0));
  DMA_TASK_ARGS(task)->src = src2;
  DMA_TASK_ARGS(task)->dest = dest2;
  DMA_TASK_ARGS(task)->unk_34 = unk34;
  DMA_TASK_ARGS(task)->unk_36 = unk36;
  DMA_TASK_ARGS(task)->callback = callback2;
  gTaskSystemReady = 1;
}
