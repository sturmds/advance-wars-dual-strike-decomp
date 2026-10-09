#include "types.h"
#include "game/sound.h"

void Task_CreateChild(void **handle, int fadeFrames);
extern void *data_02159f80;

void func_02031480(int fadeFrames)
{
    Task_CreateChild(&data_02159f80, fadeFrames);
}
