#include "types.h"

int func_0207bd80(void);
void Task_CreateChild(void **handle, int fadeFrames);
extern void *data_02160624;

void func_020811b4(int fadeFrames)
{
    if (func_0207bd80() == 1) {
        Task_CreateChild(&data_02160624, fadeFrames);
    }
}
