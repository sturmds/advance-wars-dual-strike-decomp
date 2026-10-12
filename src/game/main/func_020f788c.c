#include "types.h"

void* Task_FindByScript(void* script);
extern u8 data_02169dac[];

BOOL func_020f788c(void) { return Task_FindByScript(data_02169dac) != NULL; }
