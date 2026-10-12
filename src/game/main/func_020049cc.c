#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd data_021572a4[];
extern const TaskScriptCmd data_02157390[];
int func_02004a14(void);

void func_020049cc(u16 value) {
  Task* task;

  if (func_02004a14() == 0) {
    task = Task_Create(data_02157390, TASK_LIST(3));
  } else {
    task = Task_Create(data_021572a4, TASK_LIST(3));
  }
  if (task != NULL) {
    task->unk_64 = value;
  }
}
