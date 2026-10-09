#include "types.h"

void *Task_Create(const void *script, void *parentOrList);

extern const u32 data_02169930[];

void func_020f3ecc(void)
{
    Task_Create(data_02169930, (void *)3);
}
