#include "types.h"

void *Task_Create(const void *script, void *parent);
extern u8 data_0215b648[];

void func_0204d8c8(void)
{
    Task_Create(data_0215b648, (void *)3);
}
