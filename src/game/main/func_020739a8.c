#include "types.h"

void *Task_Create(const void *script, void *parent);
extern u8 data_0215fad0[];

void func_020739a8(void)
{
    Task_Create(data_0215fad0, (void *)3);
}
