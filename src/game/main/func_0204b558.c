#include "types.h"
#include "game/sound.h"

void Task_Create(void **handle, int fadeFrames);
extern void *data_0215b164;

void func_0204b558(int fadeFrames)
{
    Task_Create(&data_0215b164, fadeFrames);
}
