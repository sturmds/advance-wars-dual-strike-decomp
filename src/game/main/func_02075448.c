#include "types.h"

void *Task_Create(const void *script, void *parent);
extern u8 data_0215fb68[];

void func_02075448(void)
{
    Task_Create(data_0215fb68, (void *)3);
}
