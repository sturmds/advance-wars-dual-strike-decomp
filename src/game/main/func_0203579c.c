#include "types.h"

void *Task_Create(const void *script, void *parent);
extern u8 data_02159d50[];

void func_0203579c(void)
{
    Task_Create(data_02159d50, (void *)3);
}
