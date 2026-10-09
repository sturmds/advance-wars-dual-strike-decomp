#include "types.h"

void *Task_Create(const void *script, void *parent);
extern u8 data_021616c4[];

void func_0208af34(void)
{
    Task_Create(data_021616c4, (void *)3);
}
