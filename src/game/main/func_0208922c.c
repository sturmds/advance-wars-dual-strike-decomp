#include "types.h"
#include "game/sound.h"

void Task_CreateChild(void **handle, int fadeFrames);
extern void *data_021615d0;

void func_0208922c(int fadeFrames)
{
    Task_CreateChild(&data_021615d0, fadeFrames);
}
