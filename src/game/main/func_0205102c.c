#include "types.h"
#include "game/sound.h"

void Task_Create(void **handle, int fadeFrames);
extern void *data_0215bb68;

void func_0205102c(int fadeFrames)
{
    Task_Create(&data_0215bb68, fadeFrames);
}
