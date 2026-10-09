#include "types.h"
#include "game/sound.h"

void Task_CreateChild(void **handle, int fadeFrames);
extern void *data_0215fc0c;

void func_02075df0(int fadeFrames)
{
    Task_CreateChild(&data_0215fc0c, fadeFrames);
}
