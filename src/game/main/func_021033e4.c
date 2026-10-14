#include "types.h"
#include "game/task.h"

void func_02075df0(void);
void func_0208922c(void);
void func_020b983c(void);
void func_020fd8ec(void);
void func_02103714(void);

TaskScriptCmd data_0216d6bc[] = {
#include "data/main/data_0216d6bc.inc.c"
};
static TaskScriptCmd data_0216d6a4[] = {
#include "data/main/data_0216d6a4.inc.c"
};
TaskScriptCmd data_0216d6d4[] = {
#include "data/main/data_0216d6d4.inc.c"
};

void func_021033e4(void) { Task_Create(data_0216d6a4, TASK_LIST(3)); }
