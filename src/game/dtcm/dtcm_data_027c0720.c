#include "types.h"
#include "game/main_loop.h"

#pragma explicit_zero_data on
MainLoop gMainLoop = {
#include "data/dtcm/gMainLoop.inc.c"
};
#pragma explicit_zero_data reset
