#include "types.h"

void CoSelectScreen_HandleR(const void *src, void *dst, u32 size);

void func_02041aa0(const void *src, void *dst, u32 size)
{
    CoSelectScreen_HandleR(src, dst, size);
}
