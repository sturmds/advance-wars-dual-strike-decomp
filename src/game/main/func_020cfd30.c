#include "types.h"
#include "game/task.h"

void* Task_FindByScript(void* script);
extern TaskScriptCmd data_021665d0[];

BOOL func_020cfd30(void) { return Task_FindByScript(data_021665d0) != NULL; }
