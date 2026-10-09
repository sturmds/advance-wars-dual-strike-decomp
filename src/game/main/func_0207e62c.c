#include "types.h"

void *Task_Create(const void *script, void *parent);
extern u8 data_021601a0[];

void func_0207e62c(void)
{
    Task_Create(data_021601a0, (void *)3);
}
