#include "types.h"
#include "game/sound.h"

void Task_CreateChild(void **handle, int fadeFrames);
extern void *data_0216a998;

void func_020ffaec(int fadeFrames)
{
    Task_CreateChild(&data_0216a998, fadeFrames);
}
