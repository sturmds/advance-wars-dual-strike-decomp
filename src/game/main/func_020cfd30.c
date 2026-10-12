#include "types.h"

void* Task_FindByScript(void* script);
extern u8 data_021665d0[];

BOOL func_020cfd30(void) { return Task_FindByScript(data_021665d0) != NULL; }
