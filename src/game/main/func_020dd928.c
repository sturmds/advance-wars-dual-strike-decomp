#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd data_02169028[];

void func_020dd928(void) { Task_Create(data_02169028, TASK_LIST(3)); }
