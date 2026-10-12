#include "types.h"
#include "sdk/os.h"

u32 OS_GetThreadPriority(const OSThread* thread) { return thread->priority; }
