#include "types.h"

void *Task_Create(const void *script, void *parentOrList);

extern const u32 data_021674e0[];

void func_020d7eb8(void)
{
    Task_Create(data_021674e0, (void *)0);
}
