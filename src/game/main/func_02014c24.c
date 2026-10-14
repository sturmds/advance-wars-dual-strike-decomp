#include "types.h"
#include "game/task.h"

int Task_DestroyByScript(const void* script);

extern TaskScriptCmd data_02158354[];

int func_02014c24(void) { return Task_DestroyByScript(data_02158354); }
