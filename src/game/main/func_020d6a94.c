#include "types.h"

void *Task_Create(const void *script, void *parent);
extern u8 data_0216737c[];

void func_020d6a94(void)
{
    Task_Create(data_0216737c, (void *)3);
}
