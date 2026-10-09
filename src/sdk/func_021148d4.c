#include "types.h"
#include "game/sound.h"

void func_02114980(void **handle, int fadeFrames);
extern void *data_022ab8f8;

void func_021148d4(int fadeFrames)
{
    func_02114980(&data_022ab8f8, fadeFrames);
}
