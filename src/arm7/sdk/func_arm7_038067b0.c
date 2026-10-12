#include "types.h"

/* BIOS call 0x0b (CpuSet, by the caller's arguments). */
extern void func_arm7_03803bb8(const void *src, void *dst, u32 control);

void func_arm7_038067b0(const void *src, void *dst, u32 control)
{
    func_arm7_03803bb8(src, dst, control);
}
