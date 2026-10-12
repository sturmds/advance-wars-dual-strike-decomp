#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd data_ov002_0236961c[];

void func_ov002_023571b8(void) {
  Task_Create(data_ov002_0236961c, TASK_LIST(1));
}
