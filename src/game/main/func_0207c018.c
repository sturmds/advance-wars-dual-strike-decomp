#include "types.h"

void* Task_FindByScript(void* script);
extern u8 data_0215ff9c[];

BOOL func_0207c018(void) { return Task_FindByScript(data_0215ff9c) != NULL; }
