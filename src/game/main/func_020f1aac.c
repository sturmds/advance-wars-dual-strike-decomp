#include "types.h"
#include "game/sound.h"

void Task_Create(void **handle, int fadeFrames);
extern void *data_021695b8;

void func_020f1aac(int fadeFrames)
{
    Task_Create(&data_021695b8, fadeFrames);
}
