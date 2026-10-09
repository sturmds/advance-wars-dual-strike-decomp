#include "types.h"

void *Task_Create(const void *script, void *parent);
extern u8 data_02166568[];

void func_020ccd7c(void)
{
    Task_Create(data_02166568, (void *)3);
}
