#include "types.h"

void *Task_Create(const void *script, void *parent);
extern u8 data_02159b14[];

void func_0202e868(void)
{
    Task_Create(data_02159b14, (void *)3);
}
