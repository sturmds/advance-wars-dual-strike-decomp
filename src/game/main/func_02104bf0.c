#include "types.h"

void Heap_Unlink(const void *src, void *dst, u32 size);

void func_02104bf0(const void *src, void *dst, u32 size)
{
    Heap_Unlink(src, dst, size);
}
