#include "types.h"
#include "game/main_loop.h"

extern MainLoop gMainLoop;

u32 MainLoop_GetFrameCount(void) { return gMainLoop.frameCount; }
