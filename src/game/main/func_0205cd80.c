#include "types.h"
#include "game/sound.h"

void Task_CreateChild(void **handle, int fadeFrames);
extern void *data_0215c9a8;

void func_0205cd80(int fadeFrames)
{
    Task_CreateChild(&data_0215c9a8, fadeFrames);
}
