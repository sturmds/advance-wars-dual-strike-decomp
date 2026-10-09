#include "types.h"
#include "game/sound.h"

void Task_CreateChild(void **handle, int fadeFrames);
extern void *data_ov005_023694cc;

void func_ov005_02357874(int fadeFrames)
{
    Task_CreateChild(&data_ov005_023694cc, fadeFrames);
}
