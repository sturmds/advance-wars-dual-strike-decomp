#include "types.h"

void *Task_Create(const void *script, void *parent);
extern u8 data_0215fe90[];

void func_0207ab14(void)
{
    Task_Create(data_0215fe90, (void *)3);
}
