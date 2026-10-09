#include "types.h"
#include "game/sound.h"

void Task_Create(void **handle, int fadeFrames);
extern void *data_02158354;

void func_02014c38(int fadeFrames)
{
    Task_Create(&data_02158354, fadeFrames);
}
