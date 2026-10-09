#include "types.h"
#include "game/sound.h"

void Task_CreateChild(void **handle, int fadeFrames);
extern void *data_0215c4f4;

void func_02055cac(int fadeFrames)
{
    Task_CreateChild(&data_0215c4f4, fadeFrames);
}
