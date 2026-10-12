#include "types.h"

void* Task_FindByScript(void* script);
extern u8 data_021690f8[];

BOOL func_020eedf0(void) { return Task_FindByScript(data_021690f8) != NULL; }
