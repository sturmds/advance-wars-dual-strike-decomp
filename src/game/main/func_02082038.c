#include "types.h"

int func_0207bd80(void);
void Task_CreateChild(void **handle, int fadeFrames);
extern void *data_021606ec;

void func_02082038(int fadeFrames)
{
    if (func_0207bd80() == 1) {
        Task_CreateChild(&data_021606ec, fadeFrames);
    }
}
