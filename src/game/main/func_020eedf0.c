#include "types.h"
#include "game/task.h"

void* Task_FindByScript(void* script);
extern TaskScriptCmd data_021690f8[];

BOOL func_020eedf0(void) { return Task_FindByScript(data_021690f8) != NULL; }
