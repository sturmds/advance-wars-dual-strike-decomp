#include "types.h"

void func_0210f4a0(u32 value, void *dst, u32 size);   /* SDK MI_CpuFill32 */

/* Fills size bytes with value in 32-bit units. The volatile copy of value
 * is needed to match. */
void Mem_Fill32(u32 value, void *dst, u32 size)
{
    volatile u32 fill = value;

    func_0210f4a0(fill, dst, size);
}
