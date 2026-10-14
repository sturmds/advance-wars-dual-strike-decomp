#include "types.h"
#include "game/task.h"

void Campaign_Continue(void);
void Campaign_UnlockTopCos(void);
void func_0207639c(void);
void func_020fd8a4(void);
void func_020fd94c(void);
void func_020ffaec(void);
void func_0210372c(void);
void func_02103938(void);

static TaskScriptCmd data_0216d8ac[8] = {
#include "data/main/data_0216d8ac.inc.c"
};
TaskScriptCmd data_0216d8ec[8] = {
#include "data/main/data_0216d8ec.inc.c"
};

/* Starts the data_0216d8ac task on top-level task list 3. */
void func_0210375c(void) { Task_Create(data_0216d8ac, TASK_LIST(3)); }
