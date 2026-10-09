#include "types.h"
#include "game/sound.h"

void Task_CreateChild(void **handle, int fadeFrames);
extern void *data_ov005_02368230;

void func_ov005_02350560(int fadeFrames)
{
    Task_CreateChild(&data_ov005_02368230, fadeFrames);
}
