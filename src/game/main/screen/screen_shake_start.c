#include "types.h"
#include "game/task.h"

extern TaskScriptCmd gScreenShakeScript[];
extern s16* data_021571ec[];

/* Starts a screen shake task: tableIndex picks the offset table, steps is the
 * number of frames (-1: one pass of the table). Runs under parent if given. */
void ScreenShake_Start(int tableIndex, int steps, Task* parent) {
  Task* task;

  if (parent != NULL) {
    task = Task_CreateChild(gScreenShakeScript, parent);
  } else {
    task = Task_Create(gScreenShakeScript, TASK_LIST(1));
  }
  task->unk_4c = data_021571ec[tableIndex];
  task->unk_44 = steps;
}
