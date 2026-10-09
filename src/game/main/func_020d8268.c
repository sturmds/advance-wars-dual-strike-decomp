#include "types.h"

void *Task_FindByScript(void *script);
extern u8 data_021675e0[];

BOOL func_020d8268(void)
{
    return Task_FindByScript(data_021675e0) != NULL;
}
