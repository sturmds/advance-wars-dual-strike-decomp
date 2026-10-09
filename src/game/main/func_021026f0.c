#include "types.h"

void *Task_Create(const void *script, void *parent);
extern u8 data_0216d62c[];

void func_021026f0(void)
{
    Task_Create(data_0216d62c, (void *)3);
}
