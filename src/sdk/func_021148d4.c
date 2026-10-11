#include "types.h"
#include "game/sound.h"

void PowerHook_AddLast(void **handle, int fadeFrames);
extern void *data_022ab8f8;

void func_021148d4(int fadeFrames)
{
    PowerHook_AddLast(&data_022ab8f8, fadeFrames);
}
