#include "types.h"

void *Task_Create(const void *script, void *parent);
extern u8 data_0215a44c[];

void func_02038264(void)
{
    Task_Create(data_0215a44c, (void *)3);
}
