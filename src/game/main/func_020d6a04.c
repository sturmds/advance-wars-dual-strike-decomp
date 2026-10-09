#include "types.h"

void *Task_Create(const void *script, void *parent);
extern u8 data_021672b4[];

void func_020d6a04(void)
{
    Task_Create(data_021672b4, (void *)3);
}
