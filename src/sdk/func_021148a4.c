#include "types.h"
#include "game/sound.h"

void func_02114934(void **handle, int fadeFrames);
extern void *data_022ab8f8;

void func_021148a4(int fadeFrames)
{
    func_02114934(&data_022ab8f8, fadeFrames);
}
