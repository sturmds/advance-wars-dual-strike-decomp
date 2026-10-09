#include "types.h"
#include "game/sound.h"

void Task_Create(void **handle, int fadeFrames);
extern void *data_0215b06c;

void func_0204c11c(int fadeFrames)
{
    Task_Create(&data_0215b06c, fadeFrames);
}
