#include "types.h"
#include "game/sound.h"

void func_02114934(void **handle, int fadeFrames);
extern void *data_022ab8f0;

void func_021148bc(int fadeFrames)
{
    func_02114934(&data_022ab8f0, fadeFrames);
}
