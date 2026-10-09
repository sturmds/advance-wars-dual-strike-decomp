#include "types.h"

void *Task_Create(const void *script, void *parent);
extern u8 data_0216d8ec[];

void func_02103744(void)
{
    Task_Create(data_0216d8ec, (void *)3);
}
