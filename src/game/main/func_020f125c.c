#include "types.h"

void *Task_FindByScript(void *script);
extern u8 data_02169494[];

BOOL func_020f125c(void)
{
    return Task_FindByScript(data_02169494) != NULL;
}
