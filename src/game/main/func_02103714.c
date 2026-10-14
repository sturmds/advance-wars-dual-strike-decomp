#include "types.h"
#include "game/task.h"

void EventScript_Init(void);
void MainMenu_Prepare(void);
extern u8 data_0216d754[];
void func_0200a630(void);

TaskScriptCmd data_0216d6fc[] = {
#include "data/main/data_0216d6fc.inc.c"
};

void func_02103714(void) { Task_Create(data_0216d6fc, TASK_LIST(3)); }
