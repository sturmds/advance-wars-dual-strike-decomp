#include "types.h"
#include "game/sound.h"

void Task_Create(void **handle, int fadeFrames);
extern void *data_0215bbc8;

void func_0205067c(int fadeFrames)
{
    Task_Create(&data_0215bbc8, fadeFrames);
}
