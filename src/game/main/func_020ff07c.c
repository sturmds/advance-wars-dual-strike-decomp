#include "types.h"

void Map_ResetFrameCallbacks(const void *src, void *dst, u32 size);

void func_020ff07c(const void *src, void *dst, u32 size)
{
    Map_ResetFrameCallbacks(src, dst, size);
}
