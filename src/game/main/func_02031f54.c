#include "types.h"

void *Task_Create(const void *script, void *parent);
extern u8 data_02159e20[];

void func_02031f54(void)
{
    Task_Create(data_02159e20, (void *)3);
}
