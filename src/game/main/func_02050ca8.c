#include "types.h"
#include "game/sound.h"

void Task_Create(void **handle, int fadeFrames);
extern void *data_0215bba8;

void func_02050ca8(int fadeFrames)
{
    Task_Create(&data_0215bba8, fadeFrames);
}
