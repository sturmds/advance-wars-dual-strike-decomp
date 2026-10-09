#include "types.h"

void *Task_Create(const void *script, void *parent);
extern u8 data_0216d724[];

void func_0210372c(void)
{
    Task_Create(data_0216d724, (void *)3);
}
