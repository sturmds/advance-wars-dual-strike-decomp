#include "types.h"
#include "game/sound.h"

void Task_CreateChild(void **handle, int fadeFrames);
extern void *data_0216176c;

void func_0208b988(int fadeFrames)
{
    Task_CreateChild(&data_0216176c, fadeFrames);
}
