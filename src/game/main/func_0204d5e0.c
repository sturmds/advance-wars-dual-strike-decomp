#include "types.h"
#include "game/task.h"

void* Task_FindByScript(void* script);
extern TaskScriptCmd data_0215b5e8[];

BOOL func_0204d5e0(void) { return Task_FindByScript(data_0215b5e8) != NULL; }
