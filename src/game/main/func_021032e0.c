#include "types.h"

void *Task_Create(const void *script, void *parent);
extern u8 data_0216d65c[];

void func_021032e0(void)
{
    Task_Create(data_0216d65c, (void *)3);
}
