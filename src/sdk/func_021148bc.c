#include "types.h"
#include "game/sound.h"

void PowerHook_Unlink(void **handle, int fadeFrames);
extern void *data_022ab8f0;

void func_021148bc(int fadeFrames)
{
    PowerHook_Unlink(&data_022ab8f0, fadeFrames);
}
