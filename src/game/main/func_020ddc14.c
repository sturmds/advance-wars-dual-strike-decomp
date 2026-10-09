#include "types.h"

void *Task_Create(const void *script, void *parent);
extern u8 data_02166678[];

void func_020ddc14(void)
{
    Task_Create(data_02166678, (void *)3);
}
