#include "types.h"

void *Task_Create(const void *script, void *parent);
extern u8 data_02159a0c[];

void func_0202d888(void)
{
    Task_Create(data_02159a0c, (void *)3);
}
