#include "types.h"
#include "game/sound.h"

void Task_CreateChild(void **handle, int fadeFrames);
extern void *data_0216a7d0;

void func_020fd8ec(int fadeFrames)
{
    Task_CreateChild(&data_0216a7d0, fadeFrames);
}
