#include "types.h"
#include "game/sound.h"

void Task_CreateChild(void **handle, int fadeFrames);
extern void *data_ov005_02368af8;

void func_ov005_02355268(int fadeFrames)
{
    Task_CreateChild(&data_ov005_02368af8, fadeFrames);
}
