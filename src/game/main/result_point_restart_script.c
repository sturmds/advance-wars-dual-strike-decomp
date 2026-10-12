#include "types.h"
#include "game/task.h"
#include "game/Unk_0229c734.h"

ResultPointTask* Task_FindByScript(const void* script);
int Task_JumpToLabel(ResultPointTask* task, int label);

extern const TaskScriptCmd data_0215a44c[];

/* Stores value in the result point task and restarts its script at label 0. */
void ResultPoint_RestartScript(int value) {
  ResultPointTask* task = Task_FindByScript(data_0215a44c);

  data_0229c734->unk_a4 = 1;
  task->value = value;
  Task_JumpToLabel(task, 0);
}
