#include "types.h"
#include "game/main_loop.h"

extern MainLoop gMainLoop;

void* func_01ff9c38(void) { return gMainLoop.screenCallbacks[0]; }
