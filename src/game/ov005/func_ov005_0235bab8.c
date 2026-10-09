#include "types.h"
#include "game/sound.h"

void Task_CreateChild(void **handle, int fadeFrames);
extern void *data_ov005_0236a578;

void func_ov005_0235bab8(int fadeFrames)
{
    Task_CreateChild(&data_ov005_0236a578, fadeFrames);
}
