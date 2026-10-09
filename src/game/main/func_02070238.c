#include "types.h"
#include "game/sound.h"

void Task_CreateChild(void **handle, int fadeFrames);
extern void *data_0215d664;

void func_02070238(int fadeFrames)
{
    Task_CreateChild(&data_0215d664, fadeFrames);
}
