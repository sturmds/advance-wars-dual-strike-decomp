#include "types.h"

int func_0207bd80(void);
void Task_CreateChild(void **handle, int fadeFrames);
extern void *data_0216065c;

void func_020811e4(int fadeFrames)
{
    if (func_0207bd80() == 1) {
        Task_CreateChild(&data_0216065c, fadeFrames);
    }
}
