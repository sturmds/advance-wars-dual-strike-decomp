#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd data_02168c3c[];

void func_020e8848(void) { Task_Create(data_02168c3c, TASK_LIST(3)); }
