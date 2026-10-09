#include "types.h"
#include "game/sound.h"

void Task_CreateChild(void **handle, int fadeFrames);
extern void *data_ov005_0236a724;

void func_ov005_0235d72c(int fadeFrames)
{
    Task_CreateChild(&data_ov005_0236a724, fadeFrames);
}
