#include "types.h"
#include "game/task.h"
#include "game/dma_task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

extern Unk_02080440* data_027c027c;
extern Unk_027c0288 data_027c0288;
extern u32 gTaskSystemReady;
extern const TaskScriptCmd data_021571ac[]; /* DMA task on channel 0 */
extern const TaskScriptCmd data_021571cc[]; /* DMA task on channel 1 */

/* Starts one DMA task that feeds `dest` from `src`, on channel 0 if it is
 * free, else on channel 1. Does nothing when both are in use. */
void DmaTask_Start(const void* src, volatile void* dest, int unk34, int unk36,
                   void (*callback)(void)) {
  Task* task;

  gTaskSystemReady = 0;
  if (gDmaTaskState.channelScreen[0] == 0) {
    task = Task_Create(data_021571ac, TASK_LIST(0));
    if ((void*)data_027c027c == data_027c0288.unk_00) {
      gDmaTaskState.channelScreen[0] = 1;
    } else {
      gDmaTaskState.channelScreen[0] = 2;
    }
  } else if (gDmaTaskState.channelScreen[1] == 0) {
    task = Task_Create(data_021571cc, TASK_LIST(0));
    if ((void*)data_027c027c == data_027c0288.unk_00) {
      gDmaTaskState.channelScreen[1] = 1;
    } else {
      gDmaTaskState.channelScreen[1] = 2;
    }
    gDmaTaskState.channel1Callback = callback;
  } else {
    /* gTaskSystemReady stays 0 here */
    return;
  }
  DMA_TASK_ARGS(task)->src = src;
  DMA_TASK_ARGS(task)->dest = dest;
  DMA_TASK_ARGS(task)->unk_34 = unk34;
  DMA_TASK_ARGS(task)->unk_36 = unk36;
  DMA_TASK_ARGS(task)->callback = callback;
  gDmaTaskState.pair = 0;
  gTaskSystemReady = 1;
}
