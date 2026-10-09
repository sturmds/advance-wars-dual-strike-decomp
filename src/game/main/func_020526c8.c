#include "types.h"

void *Task_Create(const void *script, void *parent);
extern u8 data_0215bf84[];

void func_020526c8(void)
{
    Task_Create(data_0215bf84, (void *)3);
}
