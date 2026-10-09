#include "types.h"
#include "game/sound.h"

void Task_Create(void **handle, int fadeFrames);
extern void *data_02159ff8;

void func_02032b28(int fadeFrames)
{
    Task_Create(&data_02159ff8, fadeFrames);
}
