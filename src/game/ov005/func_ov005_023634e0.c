#include "types.h"
#include "game/sound.h"

void Task_CreateChild(void **handle, int fadeFrames);
extern void *data_ov005_0236afdc;

void func_ov005_023634e0(int fadeFrames)
{
    Task_CreateChild(&data_ov005_0236afdc, fadeFrames);
}
