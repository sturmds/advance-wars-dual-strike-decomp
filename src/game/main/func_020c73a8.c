#include "types.h"

void Player_IncUnitCount(const void *src, void *dst, u32 size);

void func_020c73a8(const void *src, void *dst, u32 size)
{
    Player_IncUnitCount(src, dst, size);
}
