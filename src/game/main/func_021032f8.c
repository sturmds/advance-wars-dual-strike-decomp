#include "types.h"

void *Task_Create(const void *script, void *parent);
extern u8 data_0216d674[];

void func_021032f8(void)
{
    Task_Create(data_0216d674, (void *)3);
}
