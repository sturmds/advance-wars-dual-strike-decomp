#include "types.h"
#include "game/sound.h"

void Task_CreateChild(void **handle, int fadeFrames);
extern void *data_ov005_0236b38c;

void func_ov005_02365ea8(int fadeFrames)
{
    Task_CreateChild(&data_ov005_0236b38c, fadeFrames);
}
