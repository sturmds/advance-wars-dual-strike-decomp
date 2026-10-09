#include "types.h"

void *Task_Create(const void *script, void *parent);
extern u8 data_0216d6a4[];

void func_021033e4(void)
{
    Task_Create(data_0216d6a4, (void *)3);
}
