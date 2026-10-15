#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/dma_task.h"
#include "game/task.h"

void Task_DestroyByScript(const void* descriptor);

extern TaskScriptCmd data_021571ac[]; /* script of the first DMA task */
extern TaskScriptCmd data_021571cc[]; /* script of the second DMA task */

/* Destroys the tasks of the DMA pair that DmaTasks_Start
 * started: both when gDmaTaskState.pair is set, else the one that belongs to
 * the current screen. */
void DmaTasks_Destroy(void) {
  if (gDmaTaskState.pair != 0) {
    Task_DestroyByScript(data_021571ac);
    Task_DestroyByScript(data_021571cc);
    return;
  }
  if ((void*)data_027c027c == data_027c0288.unk_00) {
    if (gDmaTaskState.channelScreen[0] == 1) {
      Task_DestroyByScript(data_021571ac);
    } else {
      Task_DestroyByScript(data_021571cc);
    }
  } else {
    if (gDmaTaskState.channelScreen[0] == 2) {
      Task_DestroyByScript(data_021571ac);
    } else {
      Task_DestroyByScript(data_021571cc);
    }
  }
}
