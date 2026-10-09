#include "types.h"
#include "game/sound.h"

void Task_CreateChild(void **handle, int fadeFrames);
extern void *data_0215fd64;

void func_0207639c(int fadeFrames)
{
    Task_CreateChild(&data_0215fd64, fadeFrames);
}
