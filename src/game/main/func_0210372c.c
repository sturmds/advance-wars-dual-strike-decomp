#include "types.h"
#include "game/task.h"

void EventScript_Init(void);
void GameMode_StartNext(void);
void Link_ClearPlay(void);
void MainMenu_ClearCursors(void);
void MainMenu_Prepare(void);
void MapSelect_ResetTables(void);
extern TaskScriptCmd data_02151c80[];
extern TaskScriptCmd data_0216d754[];
void func_0200a630(void);
void func_0202d6a8(void);
void func_020b983c(void);

TaskScriptCmd data_0216d724[] = {
#include "data/main/data_0216d724.inc.c"
};
TaskScriptCmd data_0216d754[] = {
#include "data/main/data_0216d754.inc.c"
};

void func_0210372c(void) { Task_Create(data_0216d724, TASK_LIST(3)); }
