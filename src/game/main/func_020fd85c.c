#include "types.h"

void *Task_CreateChild(const void *script, void *parent);

extern const u32 data_0216a6e0[];

void func_020fd85c(void *parent)
{
    Task_CreateChild(data_0216a6e0, parent);
}
