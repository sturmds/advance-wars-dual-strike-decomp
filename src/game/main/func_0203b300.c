#include "types.h"
#include "game/sound.h"

void Task_CreateChild(void **handle, int fadeFrames);
extern void *data_0215a690;

void func_0203b300(int fadeFrames)
{
    Task_CreateChild(&data_0215a690, fadeFrames);
}
