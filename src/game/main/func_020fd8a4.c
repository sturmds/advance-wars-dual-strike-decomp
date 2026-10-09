#include "types.h"
#include "game/sound.h"

void Task_CreateChild(void **handle, int fadeFrames);
extern void *data_0216a758;

void func_020fd8a4(int fadeFrames)
{
    Task_CreateChild(&data_0216a758, fadeFrames);
}
