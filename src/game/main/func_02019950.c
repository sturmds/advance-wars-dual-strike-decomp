#include "types.h"

void *Task_Create(const void *script, void *parent);
extern u8 data_02158344[];

void func_02019950(void)
{
    Task_Create(data_02158344, (void *)3);
}
