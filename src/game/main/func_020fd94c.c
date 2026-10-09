#include "types.h"
#include "game/sound.h"

void Task_CreateChild(void **handle, int fadeFrames);
extern void *data_0216a8b0;

void func_020fd94c(int fadeFrames)
{
    Task_CreateChild(&data_0216a8b0, fadeFrames);
}
