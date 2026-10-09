#include "types.h"

void *Task_Create(const void *script, void *parent);

extern u8 data_0215bbe8[];

void func_020517d8(void)
{
    Task_Create(data_0215bbe8, (void *)3);
}
