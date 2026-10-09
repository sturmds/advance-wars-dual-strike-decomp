#include "types.h"

void *Task_Create(const void *script, void *parent);
extern u8 data_0216d68c[];

void func_02102c1c(void)
{
    Task_Create(data_0216d68c, (void *)3);
}
