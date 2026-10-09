#include "types.h"
#include "game/sound.h"

void Task_CreateChild(void **handle, int fadeFrames);
extern void *data_ov005_023682e0;

void func_ov005_023536bc(int fadeFrames)
{
    Task_CreateChild(&data_ov005_023682e0, fadeFrames);
}
