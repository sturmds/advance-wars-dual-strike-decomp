#include "types.h"

void *Task_CreateChild(const void *script, void *parent);

extern const u32 data_0216a7f8[];

void func_020fd904(void *parent)
{
    Task_CreateChild(data_0216a7f8, parent);
}
