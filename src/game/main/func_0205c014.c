#include "types.h"

void *Task_CreateChild(const void *script, void *parent);

extern u8 data_0215c764[];

void func_0205c014(void *parent)
{
    Task_CreateChild(data_0215c764, parent);
}
