#include "sdk/wl.h"

/* Writes one byte by a read-modify-write of its halfword: the wireless RAM has no byte access. */
void WL_WriteByte(void *addr, u32 value)
{
    u16 *halfword;

    if ((u32)addr & 1) {
        halfword = (u16 *)((u32)addr - 1);
        *halfword = (*halfword & 0xff) | (value << 8);
    } else {
        halfword = addr;
        *halfword = (*halfword & 0xff00) | (value & 0xff);
    }
}
