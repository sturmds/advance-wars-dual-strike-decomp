#include "types.h"
#include "game/sound.h"

void Task_CreateChild(void **handle, int fadeFrames);
extern void *data_0215adec;

void func_02045890(int fadeFrames)
{
    Task_CreateChild(&data_0215adec, fadeFrames);
}
