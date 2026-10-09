#include "types.h"
#include "game/sound.h"

void Task_CreateChild(void **handle, int fadeFrames);
extern void *data_02169df8;

void func_020f5c58(int fadeFrames)
{
    Task_CreateChild(&data_02169df8, fadeFrames);
}
