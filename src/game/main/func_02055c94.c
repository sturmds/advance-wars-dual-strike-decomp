#include "types.h"
#include "game/sound.h"

void Task_CreateChild(void **handle, int fadeFrames);
extern void *data_0215c444;

void func_02055c94(int fadeFrames)
{
    Task_CreateChild(&data_0215c444, fadeFrames);
}
