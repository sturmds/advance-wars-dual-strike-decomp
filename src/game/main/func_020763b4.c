#include "types.h"

void *Task_Create(const void *script, void *parent);
extern u8 data_0215fcfc[];

void func_020763b4(void)
{
    Task_Create(data_0215fcfc, (void *)3);
}
