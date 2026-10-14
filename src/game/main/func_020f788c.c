#include "types.h"
#include "game/task.h"

void* Task_FindByScript(void* script);
extern TaskScriptCmd data_02169dac[];

BOOL func_020f788c(void) { return Task_FindByScript(data_02169dac) != NULL; }
