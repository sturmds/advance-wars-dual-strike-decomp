#include "types.h"

void *Task_Create(const void *script, void *parentOrList);

extern const u32 data_021673dc[];

void func_020d6a1c(void)
{
    Task_Create(data_021673dc, (void *)3);
}
