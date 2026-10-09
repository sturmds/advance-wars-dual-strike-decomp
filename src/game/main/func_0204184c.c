#include "types.h"
#include "game/sound.h"

void Task_CreateChild(void **handle, int fadeFrames);
extern void *data_0215ab00;

void func_0204184c(int fadeFrames)
{
    Task_CreateChild(&data_0215ab00, fadeFrames);
}
