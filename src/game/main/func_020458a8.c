#include "types.h"
#include "game/sound.h"

void Task_CreateChild(void **handle, int fadeFrames);
extern void *data_0215ad6c;

void func_020458a8(int fadeFrames)
{
    Task_CreateChild(&data_0215ad6c, fadeFrames);
}
