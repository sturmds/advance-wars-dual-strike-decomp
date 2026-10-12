#include "types.h"

void CARD_SetThreadPriority(const void *src, void *dst, u32 size);

void func_arm7_038028c8(const void *src, void *dst, u32 size)
{
    CARD_SetThreadPriority(src, dst, size);
}
