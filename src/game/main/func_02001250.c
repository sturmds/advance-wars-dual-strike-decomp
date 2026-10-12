#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd data_02151c80[];

void func_02001250(void) { Task_Create(data_02151c80, TASK_LIST(3)); }
