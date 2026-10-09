#include "types.h"

void *Task_Create(const void *script, void *parent);
extern u8 data_0215b034[];

void func_0204cce8(void)
{
    Task_Create(data_0215b034, (void *)3);
}
