#include "types.h"

void *Task_Create(const void *script, void *parent);
extern u8 data_0215bfc4[];

void func_020526e0(void)
{
    Task_Create(data_0215bfc4, (void *)3);
}
