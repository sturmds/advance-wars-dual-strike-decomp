#include "types.h"

void *Task_Create(const void *script, void *parent);
extern u8 data_021692e4[];

void func_020dda74(void)
{
    Task_Create(data_021692e4, (void *)3);
}
