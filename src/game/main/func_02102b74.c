#include "types.h"

void *Task_Create(const void *script, void *parent);
extern u8 data_0216d614[];

void func_02102b74(void)
{
    Task_Create(data_0216d614, (void *)3);
}
