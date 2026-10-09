#include "types.h"
#include "game/sound.h"

void Task_Create(void **handle, int fadeFrames);
extern void *data_021695e8;

void func_020f29d4(int fadeFrames)
{
    Task_Create(&data_021695e8, fadeFrames);
}
