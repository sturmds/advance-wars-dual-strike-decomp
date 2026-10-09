#include "types.h"

void *Task_FindByScript(void *script);
extern u8 data_02169858[];

BOOL func_020f3178(void)
{
    return Task_FindByScript(data_02169858) != NULL;
}
