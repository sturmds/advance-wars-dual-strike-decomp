#include "types.h"
#include "game/sound.h"

void Task_CreateChild(void **handle, int fadeFrames);
extern void *data_02159c78;

void func_020329c0(int fadeFrames)
{
    Task_CreateChild(&data_02159c78, fadeFrames);
}
