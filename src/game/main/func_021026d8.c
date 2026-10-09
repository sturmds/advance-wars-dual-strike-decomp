#include "types.h"

void *Task_Create(const void *script, void *parent);
extern u8 data_0216d6bc[];

void func_021026d8(void)
{
    Task_Create(data_0216d6bc, (void *)3);
}
