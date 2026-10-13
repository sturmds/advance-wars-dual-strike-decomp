#include "types.h"
#include "sdk/msl.h"

int gMslLockCounts[MSL_LOCK_COUNT];
u32 gMslLockOwners[MSL_LOCK_COUNT];
