#include "types.h"

int func_0207bd80(void);
void Task_CreateChild(void **handle, int fadeFrames);
extern void *data_02159988;

void func_0202cbcc(int fadeFrames)
{
    if (func_0207bd80() == 1) {
        Task_CreateChild(&data_02159988, fadeFrames);
    }
}
