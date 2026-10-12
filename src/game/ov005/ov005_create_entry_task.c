#include "types.h"
#include "game/task.h"
#include "game/ov005_task.h"

extern const TaskScriptCmd gOv005EntryTaskScript[];

Ov005Task* Ov005_CreateEntryTask(void* work, int entryIndex, int arg) {
  Ov005Task* task =
      (Ov005Task*)Task_Create(gOv005EntryTaskScript, TASK_LIST(3));

  task->work = work;
  task->entryIndex = entryIndex;
  task->unk_48 = arg;
  task->step = 0;
  task->unk_30 = 1;
  return task;
}
