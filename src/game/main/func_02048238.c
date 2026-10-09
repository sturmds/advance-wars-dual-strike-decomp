#include "types.h"
#include "game/sound.h"

void Task_CreateChild(void **handle, int fadeFrames);
extern void *data_0215af10;

void func_02048238(int fadeFrames)
{
    Task_CreateChild(&data_0215af10, fadeFrames);
}
