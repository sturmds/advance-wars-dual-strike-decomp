#include "types.h"
#include "game/task.h"

extern u8 data_0216d724[];
void func_020fd934(void);

TaskScriptCmd data_02151c80[4] = {
#include "data/main/data_02151c80.inc.c"
};

void func_02001250(void) { Task_Create(data_02151c80, TASK_LIST(3)); }
