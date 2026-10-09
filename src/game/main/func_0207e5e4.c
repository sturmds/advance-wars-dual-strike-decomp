#include "types.h"

void *Task_Create(const void *script, void *parent);
extern u8 data_02160278[];

void func_0207e5e4(void)
{
    Task_Create(data_02160278, (void *)3);
}
