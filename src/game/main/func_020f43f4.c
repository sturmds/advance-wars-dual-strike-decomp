#include "types.h"
#include "game/sound.h"

void Task_CreateChild(void **handle, int fadeFrames);
extern void *data_021698d0;

void func_020f43f4(int fadeFrames)
{
    Task_CreateChild(&data_021698d0, fadeFrames);
}
