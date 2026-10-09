#include "types.h"
#include "game/sound.h"

void Task_CreateChild(void **handle, int fadeFrames);
extern void *data_0215b22c;

void func_02048d98(int fadeFrames)
{
    Task_CreateChild(&data_0215b22c, fadeFrames);
}
