#include "types.h"

void *Task_Create(const void *script, void *parent);
extern u8 data_0216d6fc[];

void func_02103714(void)
{
    Task_Create(data_0216d6fc, (void *)3);
}
