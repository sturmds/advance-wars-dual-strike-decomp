#include "types.h"

void *Task_Create(const void *script, void *parent);
extern u8 data_02168c3c[];

void func_020e8848(void)
{
    Task_Create(data_02168c3c, (void *)3);
}
