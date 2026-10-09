#include "types.h"

s8 FrameCallback_Remove(const void *descriptor);
extern u8 func_ov003_02353ef4[];
extern u8 func_ov003_02353d14[];

void func_ov003_02353bac(void)
{
    FrameCallback_Remove(func_ov003_02353ef4);
    FrameCallback_Remove(func_ov003_02353d14);
}
