#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd data_02157224[];

void ScreenEffect_SetBrightenMode(Task* task);

void func_02004a7c(u16 arg) {
  Task* task = Task_Create(data_02157224, TASK_LIST(3));

  if (task != NULL) {
    task->unk_64 = arg;
    ScreenEffect_SetBrightenMode(task);
  }
}
