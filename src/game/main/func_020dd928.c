#include "types.h"

void *Task_Create(const void *script, void *parent);
extern u8 data_02169028[];

void func_020dd928(void)
{
    Task_Create(data_02169028, (void *)3);
}
