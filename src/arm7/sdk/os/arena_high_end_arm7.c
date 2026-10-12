#include "types.h"
#include "sdk/os_arm7.h"

void *Arena_HighEnd_Arm7(int id)
{
    return ARENA_BOUNDS_TABLE->hi[id];
}
