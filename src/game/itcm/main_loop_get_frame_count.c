#include "types.h"
#include "game/main_loop.h"

extern MainLoop data_027c0720;

u32 MainLoop_GetFrameCount(void)
{
    return data_027c0720.frameCount;
}
