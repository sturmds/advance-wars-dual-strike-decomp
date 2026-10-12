#include "types.h"
#include "sdk/os_arm7.h"

void* Arena_LowEnd_Arm7(int id) { return ARENA_BOUNDS_TABLE->lo[id]; }
