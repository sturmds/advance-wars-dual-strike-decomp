#include "types.h"
#include "game/sound.h"

void PowerHook_AddFirst(void **handle, int fadeFrames);
extern void *data_022ab8f0;

void func_021148ec(int fadeFrames)
{
    PowerHook_AddFirst(&data_022ab8f0, fadeFrames);
}
