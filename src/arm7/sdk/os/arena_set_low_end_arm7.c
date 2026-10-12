#include "types.h"
#include "sdk/os_arm7.h"

void Arena_SetLowEnd_Arm7(int id, void* lo) { ARENA_BOUNDS_TABLE->lo[id] = lo; }
