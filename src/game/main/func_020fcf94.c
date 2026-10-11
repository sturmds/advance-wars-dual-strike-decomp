#include "types.h"

void BrightnessFade_Update(const void *src, void *dst, u32 size);

void func_020fcf94(const void *src, void *dst, u32 size)
{
    BrightnessFade_Update(src, dst, size);
}
