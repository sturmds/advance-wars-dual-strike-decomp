#include "types.h"
#include "game/sound.h"

void Task_CreateChild(void **handle, int fadeFrames);
extern void *data_0215bc10;

void func_02050664(int fadeFrames)
{
    Task_CreateChild(&data_0215bc10, fadeFrames);
}
