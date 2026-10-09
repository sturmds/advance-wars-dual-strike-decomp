#include "types.h"
#include "game/sound.h"

void Task_CreateChild(void **handle, int fadeFrames);
extern void *data_02159b44;

void func_0202d6a8(int fadeFrames)
{
    Task_CreateChild(&data_02159b44, fadeFrames);
}
