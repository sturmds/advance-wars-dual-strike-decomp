#include "types.h"

void Player_DecUnitCount(const void *src, void *dst, u32 size);

void func_020c73b4(const void *src, void *dst, u32 size)
{
    Player_DecUnitCount(src, dst, size);
}
