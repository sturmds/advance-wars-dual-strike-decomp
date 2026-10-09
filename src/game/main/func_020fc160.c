#include "types.h"
#include "game/sound.h"

void Task_CreateChild(void **handle, int fadeFrames);
extern void *data_0216a4ec;

void func_020fc160(int fadeFrames)
{
    Task_CreateChild(&data_0216a4ec, fadeFrames);
}
