#include "types.h"

void *Task_Create(const void *script, void *parent);

extern u8 data_02151c80[];

void func_02001250(void)
{
    Task_Create(data_02151c80, (void *)3);
}
