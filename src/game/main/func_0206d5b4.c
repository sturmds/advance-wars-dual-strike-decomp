#include "types.h"

void *Task_Create(const void *script, void *parent);
extern u8 data_0215d3d0[];

void func_0206d5b4(void)
{
    Task_Create(data_0215d3d0, (void *)3);
}
