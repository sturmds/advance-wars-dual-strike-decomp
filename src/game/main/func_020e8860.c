#include "types.h"

void *Task_Create(const void *script, void *parentOrList);

extern const u32 data_02168bc4[];

void func_020e8860(void)
{
    Task_Create(data_02168bc4, (void *)3);
}
