#include "types.h"
#include "game/BattleCellTask.h"

/* Puts a cell task back to its start state and forgets its sprites. */
void BattleCellTask_Reset(BattleCellTask* task) {
  task->unk_50 = 0;
  task->angle = 0;
  task->phase = task->cell * 16;
  task->moving = 0;
  task->hasShadow = 0;
  task->flashing = 0;
  task->flag3 = 0;
  task->unk_5a_lo = 0;
  CELL_TASK_ENTRY(task).sprites[0] = NULL;
  CELL_TASK_ENTRY(task).sprites[1] = NULL;
  CELL_TASK_ENTRY(task).sprites[2] = NULL;
  CELL_TASK_ENTRY(task).sprites[3] = NULL;
  task->shadow = NULL;
}
