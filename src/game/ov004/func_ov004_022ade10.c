#include "types.h"
#include "game/sound.h"

void Task_CreateChild(void **handle, int fadeFrames);
extern void *data_ov004_022be9f8;

void func_ov004_022ade10(int fadeFrames)
{
    Task_CreateChild(&data_ov004_022be9f8, fadeFrames);
}
