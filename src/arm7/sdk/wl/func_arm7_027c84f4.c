#include "types.h"
#include "sdk/mi.h"

/* Copies count halfwords from src to dest. */
void func_arm7_027c84f4(u16 *dest, const u16 *src, u32 count)
{
    Mem_CopyHalfwords_Arm7(src, dest, count << 1);
}
