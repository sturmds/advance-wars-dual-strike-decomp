#include "types.h"

void Map_ResetFrameCallbacks(const void *src, void *dst, u32 size);

void func_020d3310(const void *src, void *dst, u32 size)
{
    Map_ResetFrameCallbacks(src, dst, size);
}
