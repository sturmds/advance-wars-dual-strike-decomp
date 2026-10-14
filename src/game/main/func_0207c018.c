#include "types.h"
#include "game/task.h"

void* Task_FindByScript(void* script);
extern TaskScriptCmd data_0215ff9c[];

BOOL func_0207c018(void) { return Task_FindByScript(data_0215ff9c) != NULL; }
