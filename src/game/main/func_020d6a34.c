#include "types.h"

int func_0207bd80(void);
void Task_CreateChild(void **handle, int fadeFrames);
extern void *data_021673a4;

void func_020d6a34(int fadeFrames)
{
    if (func_0207bd80() == 1) {
        Task_CreateChild(&data_021673a4, fadeFrames);
    }
}
