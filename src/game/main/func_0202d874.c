#include "types.h"
#include "game/task.h"

int Task_DestroyByScript(const void* script);

extern TaskScriptCmd data_02159a0c[];

int func_0202d874(void) { return Task_DestroyByScript(data_02159a0c); }
