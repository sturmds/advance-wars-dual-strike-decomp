#include "types.h"

void *Task_Create(const void *script, void *parent);
extern u8 data_021602d0[];

void func_0207dc60(void)
{
    Task_Create(data_021602d0, (void *)3);
}
