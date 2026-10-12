#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd data_02169930[];

void func_020f3ecc(void) { Task_Create(data_02169930, TASK_LIST(3)); }
